#ifndef CAMCONTAINER_H
#define CAMCONTAINER_H

#include <QObject>
#include "Basesecutirycamera.h"
#include "securitycamera.h"
#include <QList>

class activeCameraGrid;

class camContainer : public QObject
{
    Q_OBJECT

public:
    explicit camContainer(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    ~camContainer()
    {
    }

    baseSecurityCamera* at(int index) const
    {
        if(index < 0 || index >= size()) {return nullptr;}

        return CameraList.at(index);
    }

    template<typename T>
    void addCamera(QUrl url) //создать новую камеру
    {
        baseSecurityCamera* cam = new T(this, url);
        CameraList.append(cam);
    }

    template<typename T>
    void append(T* cam) //пепредать существующую камеру
    {
        if(cam == nullptr) return;
        baseSecurityCamera* base = static_cast<baseSecurityCamera*>(cam);
        cam->setParent(this);
        CameraList.append(base);
    }

    void remove(int index)
    {
        if(index < 0 || index >= size()) return;
        baseSecurityCamera* cam = CameraList.takeAt(index);
        cam->deleteLater();
    }

    int size() const
    {
        return CameraList.size();
    }

    void requestActivation(int index, int x, int y)
    {
        baseSecurityCamera* cam = at(index);
        if (cam) {
            emit activateRequested(cam, x, y);
        }
    }

signals:
    void activateRequested(baseSecurityCamera* cam, int ActiveSlotX, int ActiveSlotY);

private:
    QList<baseSecurityCamera*> CameraList;

};

#endif // CAMCONTAINER_H