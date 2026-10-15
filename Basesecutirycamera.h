#include <QImage>
#include <QObject>
#include <QUrl>
#include <Qstring>



#ifndef BASESECURITYCAMERA_H
#define BASESECURITYCAMERA_H


class baseSecurityCamera : public QObject{
    Q_OBJECT

public:
    enum class camState{
        unconfigured,
        connecting,
        online,
        error,
        reconnecting,
        disconnected
    };
    Q_ENUM(camState);

    explicit baseSecurityCamera(QObject* p, const QUrl& URL);
    virtual ~baseSecurityCamera() = default;

    camState getState() const;

    virtual void setUrl( const QUrl& u);
    QUrl getUrl() const;

protected:
    //per-camera data here, camera life cycle
    QUrl url;
    camState state = camState::unconfigured;

    void setState(const baseSecurityCamera::camState s);

public slots:
    virtual void play() = 0; // lifecycle runs
    virtual void stop() = 0; //camera is completely unresponsive - no updates happen

signals:
    void errorOccurred(const QString& errorString);
    void stateChanged(const baseSecurityCamera::camState st);
    void decodedFrame(const QImage& decodedFrame);
};

using CamState = baseSecurityCamera::camState;

#endif

