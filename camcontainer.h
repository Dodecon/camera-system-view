#ifndef CAMCONTAINER_H
#define CAMCONTAINER_H

#include <QObject>

#include "securitycamera.h"
#include <QVideoWidget>
#include <QGridLayout>
#include <QGridLayout>
#include "QQueue"

class camContainer : public QObject
{
    Q_OBJECT
private:
    QList<securityCamera*> CameraList;
    QQueue<securityCamera*> camsToPlay;

private slots:
    void startNextCam();

public:
    explicit camContainer(QObject *parent = nullptr);
    ~camContainer();

    securityCamera* at(int index);

    void schedule(securityCamera* cam);

    void addCamera(QUrl url, QGridLayout* layout); //создать новую камеру
    void append(securityCamera* cam); //пепредать существующую камеру

    void remove(int index);
    void remove();

    int size();

private:
    void onCameraReady();

};

#endif // CAMCONTAINER_H
