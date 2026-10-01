#include "securitycamera.h"

#include <QTimer>
#include <QVideoFrame>
#include <QVideoSink>

securityCamera::securityCamera(const QUrl &u, QObject* p)
    : QObject(p), URL(u), videoWidget(nullptr)
{

    player = new QMediaPlayer(this);
    player->setSource(URL);

    // QAudioOutput* audioOut = new QAudioOutput(this);
    // audioOut->setVolume(0);
    // player->setAudioOutput(audioOut);

    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, &securityCamera::onMediaStatusChanged);

    connect(player, &QMediaPlayer::errorOccurred,
            this, &securityCamera::onPlayerError);
}


securityCamera::~securityCamera()
{
}



QUrl securityCamera::getUrl() const{
    return this->URL;
}

void securityCamera::setUrl(const QUrl& url) {
    this->URL = url;
    player->setSource(URL);

    disconnect(player, &QMediaPlayer::mediaStatusChanged,
                        this, &securityCamera::onMediaStatusChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged,
                     this, &securityCamera::onMediaStatusChanged);

}



QMediaPlayer::PlaybackState securityCamera::getState(){
    return player->playbackState();
}



void securityCamera::setVideoOutput(QVideoWidget* widget){
    videoWidget = widget;
    if(widget) {
        player->setVideoOutput(widget);
    }
}



QVideoWidget* securityCamera::getVideoWidget(){
    return videoWidget;
}



void securityCamera::play(){
    if(URL.isEmpty() || !URL.isValid()){
        emit errorOccurred("невалидная ссылка");
        emit feedRecieved(false);
        emit connectionAttempted();
        return;
    }
    player->play();
}

void securityCamera::pause(){
    player->pause();
}

void securityCamera::idle(){
    player->stop();
    emit feedRecieved(false);
}



void securityCamera::onPlayerError(QMediaPlayer::Error error, const QString &errorString){
    emit errorOccurred(errorString);
    emit feedRecieved(false);
    emit connectionAttempted();
}


void securityCamera::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {

    if((status == QMediaPlayer::BufferedMedia || status == QMediaPlayer::LoadedMedia)
        && player->playbackState() == QMediaPlayer::PlayingState){

        emit connectionAttempted();
        emit feedRecieved(true);

        disconnect(player, &QMediaPlayer::mediaStatusChanged,
                   this, &securityCamera::onMediaStatusChanged);

    }

    if (status == QMediaPlayer::InvalidMedia ||
        status == QMediaPlayer::NoMedia) {

        emit connectionAttempted();
        emit feedRecieved(false);
        disconnect(player, &QMediaPlayer::mediaStatusChanged,
                   this, &securityCamera::onMediaStatusChanged);
    }
}
