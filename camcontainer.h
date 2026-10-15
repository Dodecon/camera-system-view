#ifndef CAMCONTAINER_H
#define CAMCONTAINER_H

#include <QObject>
#include "securitycamera.h"
#include <QList>

class camContainer : public QObject
{
    Q_OBJECT

public:
    explicit camContainer(QObject *parent = nullptr);
    ~camContainer();

    securityCamera* at(int index) const;

     void addCamera(QUrl url); //создать новую камеру
     void append(securityCamera* cam); //пепредать существующую камеру

    void remove(int index);
    void remove(); // removes the last element

    int size() const;

signals:
    void activate(securityCamera* cam, int ActiveSlot);

private:
    QList<securityCamera*> CameraList;

};

#endif // CAMCONTAINER_H





#ifndef ACTIVECAMERAGRID_H
#define ACTIVECAMERAGRID_H

#include <QObject>
#include "securitycamera.h"
#include "cameraview.h"
#include <QList>

class activeCameraGrid: public QObject{
    Q_OBJECT

private:
    QList<securityCamera*>  activeCams{nullptr};
    QList<cameraView*> views{nullptr};
    QList<QMetaObject::Connection> conns{};

    void init(); // will be called in the constructor to create views and other setup

public slots:
    void setActive(securityCamera* cam, int ActiveSlot);

public:
    int viewCount;

    explicit activeCameraGrid(QObject* p, int VCount);
    ~activeCameraGrid();

    securityCamera* CameraAt(int index) const;
    cameraView* viewAt(int slot) const;

    void deActivate(int ActiveSlot);
};

#endif