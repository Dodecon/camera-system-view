#include "Basesecutirycamera.h"
#include <QThread>
#include <atomic>

extern "C"{
struct AVFormatContext;

struct AVDictionary;

struct AVCodecContext;
struct AVCodecParameters;
struct AVCodec;

struct SwsContext;

struct AVPacket;
struct AVFrame;
}


#ifndef SECURITYCAMERA_H
#define SECURITYCAMERA_H


class securityCamera : public baseSecurityCamera
{
    Q_OBJECT

public:
    explicit securityCamera(QObject* p, const QUrl& URL);
    ~securityCamera();

    void setUrl(const QUrl& u) override;


public slots:
    void play() override;
    void stop() override;

private:
    QThread* m_thread = nullptr;

    std::atomic<bool> stopRequested{false};
    int videoStreamID = -1;

    AVDictionary* opts = nullptr;

    AVFormatContext* format_ctx = nullptr;

    AVCodecContext* codec_ctx = nullptr;
    const AVCodec* codec = nullptr;

    SwsContext* sws = nullptr;


    static int interruptCallback(void* opaque); // gets called by Ffmpeg periodically when reading the input

    void clearData();

    void Connect();
    void Decode();

};

#endif // SECURITYCAMERA_H
