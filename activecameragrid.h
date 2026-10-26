#ifndef ACTIVECAMERAGRID_H
#define ACTIVECAMERAGRID_H

#include <QObject>
#include "Basesecutirycamera.h"
#include "view.h"
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

    QMap<QPair<int, int>, QPointer<baseSecurityCamera>> activeCams;
    QMap<QPair<int, int>, QMetaObject::Connection> conns;
    QMap<baseSecurityCamera*, QMetaObject::Connection> destroyedConns; //stores connections whose only purpose is to clean up everything
    //if a camera is deleted even outside activeCameraGrid

    bool withinBounds(int x, int y) const;

public slots:
    void setActive(baseSecurityCamera* cam, int x, int y);

public:
    explicit activeCameraGrid(QObject* p, int VCountX = 0, int VCountY = 0, QPointer<QGridLayout> outputGridLayout = nullptr);
    ~activeCameraGrid();

    baseSecurityCamera* activeCameraAt(int x, int y) const;
    View* viewAt(int x, int y) const;
    QMetaObject::Connection connectionAt(int x, int y) const;

    QGridLayout* getViewGrid() const;

    void deActivate(int x, int y);
};

#endif


