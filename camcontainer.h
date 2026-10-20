#ifndef ACTIVECAMERAGRID_H
#define ACTIVECAMERAGRID_H

#include <QObject>
#include "securitycamera.h"
#include "cameraview.h"
#include <QMap>
#include <QPair>
#include <QPointer>
#include "QGridLayout"

class activeCameraGrid: public QObject{
    Q_OBJECT

private:
    QPointer<QGridLayout> viewGrid;
    int viewCountX;
    int viewCountY;

    QMap<QPair<int, int>, QPointer<securityCamera>> activeCams;
    QMap<QPair<int, int>, QMetaObject::Connection> conns;
    QMap<securityCamera*, QMetaObject::Connection> destroyedConns; //stores connections whose only purpose is to clean up everything
                                                                   //if a camera is deleted even outside activeCameraGrid

    bool withinBounds(int x, int y) const;

public slots:
    void setActive(securityCamera* cam, int x, int y);

public:
    explicit activeCameraGrid(QObject* p, int VCountX = 0, int VCountY = 0, QPointer<QGridLayout> outputGridLayout = nullptr);
    ~activeCameraGrid();

    securityCamera* activeCameraAt(int x, int y) const;
    cameraView* viewAt(int x, int y) const;
    QMetaObject::Connection connectionAt(int x, int y) const;

    QGridLayout* getViewGrid() const;

    void deActivate(int x, int y);
};

#endif







#ifndef CAMCONTAINER_H
#define CAMCONTAINER_H

#include <QObject>
#include "securitycamera.h"
#include <QList>

class activeCameraGrid;

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

    int size() const;

    void requestActivation(int index, int x, int y);

signals:
    void activateRequested(securityCamera* cam, int ActiveSlotX, int ActiveSlotY);

private:
    QList<securityCamera*> CameraList;

};

#endif // CAMCONTAINER_H
