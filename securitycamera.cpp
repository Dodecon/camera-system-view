#include "securitycamera.h"
#include "chrono"
#include <thread>

extern "C"{
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

securityCamera::securityCamera(QObject* p, const QUrl& URL) : baseSecurityCamera::baseSecurityCamera(p, URL){

}

securityCamera::~securityCamera(){

    stopRequested.store(true);
    clearData();
}


int securityCamera::interruptCallback(void* opaque){

    auto var = static_cast<securityCamera*>(opaque);
    return var->stopRequested.load(); // 1 = ffmpeg wil stop
}

void securityCamera::clearData(){

    if (m_thread && QThread::currentThread() != m_thread) {
        m_thread->wait();
        m_thread = nullptr;
    }
    avformat_close_input(&format_ctx);
    avcodec_free_context(&codec_ctx);
    av_dict_free(&opts);
    if (sws) { sws_freeContext(sws); sws = nullptr; }
}


void securityCamera::Connect(){
    setState(CamState::connecting);

    av_dict_set(&opts, "rtsp_transport", "tcp", 0);
    av_dict_set(&opts, "timeout", "5000000", 0);
    av_dict_set(&opts, "max_delay", "100000", 0);

    format_ctx = avformat_alloc_context();


    //setting up callback, so when the signal to stop the camera is recieved, it drops instantly
    format_ctx->interrupt_callback.callback = &securityCamera::interruptCallback;
    format_ctx->interrupt_callback.opaque = this; //passing the current camera as a param to interupt callback


    QByteArray urlBytes = url.toEncoded();
    int error = avformat_open_input(&format_ctx, urlBytes.constData(), nullptr, &opts);
    av_dict_free(&opts);

    if(error < 0){
        //error
        setState(CamState::error);
        return;
    }

    error = avformat_find_stream_info(format_ctx, nullptr);

    if(error < 0){
        //error
        setState(CamState::error);
        return;
    }

    //finding the video stream
    for(int i = 0; i < format_ctx->nb_streams; i++){

        if(format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamID = i;
            break;
        }
    }

    if (videoStreamID < 0) { setState(CamState::error); return; }

    //preparing the decoder
    codec = avcodec_find_decoder(format_ctx->streams[videoStreamID]->codecpar->codec_id);
    if (!codec) { setState(CamState::error); return; }

    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) { setState(CamState::error); return; }

    if(avcodec_parameters_to_context(codec_ctx, format_ctx->streams[videoStreamID]->codecpar) < 0) {
        setState(CamState::error);
        return;
    }

    if(avcodec_open2(codec_ctx, codec, nullptr) < 0){
        setState(CamState::error);
        return;
    }

    //preparing image converter to rgb24
    sws = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws) return;
}


void securityCamera::Decode(){


    AVPacket* pkt = av_packet_alloc();
    AVFrame* raw_frame = av_frame_alloc();

    using clock = std::chrono::steady_clock;

    int64_t firstPTS = AV_NOPTS_VALUE;
    clock::time_point firstPtsArrived;



    while (!stopRequested.load() && av_read_frame(format_ctx, pkt) >= 0) {

        if (pkt->stream_index == videoStreamID) {

            if (avcodec_send_packet(codec_ctx, pkt) < 0) {

                av_packet_unref(pkt);
                continue;
            }


            while (avcodec_receive_frame(codec_ctx, raw_frame) == 0) {


                QImage img = QImage(codec_ctx->width, codec_ctx->height, QImage::Format_RGB888);
                uint8_t* destData[4] = { img.bits(), nullptr, nullptr, nullptr };
                int destLinesize[4] = { (int)img.bytesPerLine(), 0, 0, 0 };

                sws_scale(sws,
                          raw_frame->data, raw_frame->linesize,
                          0, codec_ctx->height,
                          destData, destLinesize);
                // rgb_frame now contains the usable RGB data.


                AVRational timeBase = format_ctx->streams[videoStreamID]->time_base;
                int64_t presentationTimestamp = raw_frame->best_effort_timestamp;

                if(firstPTS == AV_NOPTS_VALUE && presentationTimestamp != AV_NOPTS_VALUE){
                    firstPTS = presentationTimestamp;
                    firstPtsArrived = clock::now();
                }

                if(presentationTimestamp != AV_NOPTS_VALUE){

                    double frameOffsetSeconds = (presentationTimestamp - firstPTS) * av_q2d(timeBase); // how much time from the start of the video must pass until this frame is shown
                    std::chrono::duration<double> frameDurationOffset(frameOffsetSeconds);

                    std::this_thread::sleep_until(firstPtsArrived + frameDurationOffset);
                }


                emit decodedFrame(img);
            }
        }

        av_packet_unref(pkt);
    }


    // Flush the decoder by sending a NULL packet
    avcodec_send_packet(codec_ctx, nullptr);
    while (avcodec_receive_frame(codec_ctx, raw_frame) == 0) {
        QImage img(codec_ctx->width, codec_ctx->height, QImage::Format_RGB888);
        uint8_t* destData[4] = { img.bits(), nullptr, nullptr, nullptr };
        int destLinesize[4] = { (int)img.bytesPerLine(), 0, 0, 0 };

        sws_scale(sws, raw_frame->data, raw_frame->linesize, 0, codec_ctx->height, destData, destLinesize);
        emit decodedFrame(img);
    }

    av_packet_free(&pkt);
    av_frame_free(&raw_frame);
}



void securityCamera::play(){
    if(m_thread) return;

    //write re-connection logic here
    videoStreamID = -1;
    stopRequested.store(false);

    m_thread = QThread::create([this](){
        Connect();
        if (videoStreamID < 0 || !codec_ctx || !sws) {
            setState(CamState::reconnecting);
            return;
        }
        setState(CamState::online);
        Decode();
        setState(CamState::disconnected);
    });

    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
    m_thread->start();
}

void securityCamera::stop(){

    stopRequested.store(true);

    clearData();

    setState(CamState::disconnected);
}

void securityCamera::setUrl(const QUrl& u){
    this->stop();
    this->url = u;
    this->play();
}