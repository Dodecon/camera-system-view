#ifndef SECURITYCAMERA_H
#define SECURITYCAMERA_H

#include <QVideoWidget>
#include <QObject>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QVideoSink>


class securityCamera : public QObject
{
    Q_OBJECT


private:

    QMediaPlayer* player;
    QVideoWidget* videoWidget;

    QUrl URL;

public slots:
    void pause();
    void play();
    void idle();

private slots:
    void onPlayerError(QMediaPlayer::Error error, const QString &errorString);

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

signals:
    void connectionAttempted();
    void feedRecieved(bool recieved);

    void errorOccurred(const QString& errorString);


public:

    explicit securityCamera(const QUrl &u, QObject* p = nullptr);
    ~securityCamera();

    QUrl getUrl() const;
    void setUrl(const QUrl& url);

    QMediaPlayer::PlaybackState getState();


    QVideoWidget* getVideoWidget();

    void setVideoOutput(QVideoWidget* widget);
};

#endif // SECURITYCAMERA_H
