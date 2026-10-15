#include "camcontainer.h"

camContainer::camContainer(QObject *parent)
    : QObject(parent)
{
}

camContainer::~camContainer()
{
}

securityCamera* camContainer::at(int index) const
{
    return nullptr;
}

void camContainer::addCamera(QUrl url)
{
    Q_UNUSED(url);
}

void camContainer::append(securityCamera* cam)
{
    Q_UNUSED(cam);
}

void camContainer::remove(int index)
{
    Q_UNUSED(index);
}

void camContainer::remove()
{
}

int camContainer::size() const
{
    return 0;
}






activeCameraGrid::activeCameraGrid(QObject* p, int VCount)
    : QObject(p),
    viewCount(VCount)
{
    init();
}

activeCameraGrid::~activeCameraGrid()
{
}

void activeCameraGrid::init()
{

}

void activeCameraGrid::setActive(securityCamera* cam, int ActiveSlot)
{
    Q_UNUSED(cam);
    Q_UNUSED(ActiveSlot);
}

securityCamera* activeCameraGrid::CameraAt(int index) const
{
    Q_UNUSED(index);
    return nullptr;
}

cameraView* activeCameraGrid::viewAt(int slot) const
{
    Q_UNUSED(slot);
    return nullptr;
}

void activeCameraGrid::deActivate(int ActiveSlot)
{
    Q_UNUSED(ActiveSlot);
}