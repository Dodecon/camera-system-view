#include "activecameragrid.h"


activeCameraGrid::activeCameraGrid(QObject* p, int VCountX, int VCountY, QPointer<QGridLayout> outputGridLayout)
    : QObject(p),
    viewCountX(VCountX),
    viewCountY(VCountY),
    viewGrid(outputGridLayout)
{
    if(viewGrid == nullptr){
        //if the user doesnt provide the output grid - we create a default one
        viewGrid = new QGridLayout();
        viewGrid->setSpacing(4);
    }

    for(int y = 0; y < viewCountY; y++){
        for(int x = 0; x < viewCountX; x++){

            View* video = new View();
            QSizePolicy sp1 = video->sizePolicy();
            sp1.setRetainSizeWhenHidden(true);
            video->setSizePolicy(sp1);

            viewGrid->addWidget(video, y, x); //qt uses rows-first. code does the reverse, meaning X = columns, y = rows
        }
    }
}

activeCameraGrid::~activeCameraGrid()
{
    for (auto c : conns)          QObject::disconnect(c);
    for (auto c : destroyedConns) QObject::disconnect(c);
    conns.clear();
    destroyedConns.clear();
    activeCams.clear();
}


bool activeCameraGrid::withinBounds(int x, int y) const{    //prevent the user from tying to access data outside the Qmaps range

    if(x >= 0 && x < viewCountX && y >= 0 && y < viewCountY){
        return true;
    }
    return false;
}


void activeCameraGrid::setActive(baseSecurityCamera* cam, int x, int y)
{
    if(withinBounds(x,y)){

        QPair<int,int> gridCoord = QPair<int,int>(x,y); //track camera that outputs to the corresponding view at (x,y)


        // If this camera is already on the grid, remove it from its current slot.
        if (cam) {
            for (auto it = activeCams.begin(); it != activeCams.end(); it++) {
                if (it.value().data() == cam) {
                    deActivate(it.key().first, it.key().second);
                    break;   // one camera, one slot — can't be more than one match
                }
            }
        }

        deActivate(x, y); //Clear the target slot (evict whatever camera is there).


        View* view = viewAt(x,y); //track the connection between a camera and view at (x,y)
        if (!view) return;   // shouldn't happen after init, but defensively safe
        if (!cam) return;

        activeCams[gridCoord] = cam;


        conns[gridCoord] = connect(cam, &baseSecurityCamera::decodedFrame, view, &View::receiveFrame);

        // a lambda that cleans up all the connections a camera had if this cameras destructor was called
        destroyedConns[cam] = connect(cam, &QObject::destroyed, this, [this, cam, gridCoord]{

            if (activeCams.value(gridCoord).data() == cam) {
                if (auto c = conns.take(gridCoord))
                    QObject::disconnect(c);
                if (auto* v = viewAt(gridCoord.first, gridCoord.second))
                    v->clear();
                activeCams.remove(gridCoord);
            }
            destroyedConns.remove(cam);
        });


    }

}


baseSecurityCamera* activeCameraGrid::activeCameraAt(int x, int y) const
{
    if(withinBounds(x,y)){
        return activeCams.value(QPair<int,int>(x,y)).data();
    }
    return nullptr;

}

View* activeCameraGrid::viewAt(int x, int y) const
{
    if(withinBounds(x,y)){

        if (!viewGrid) return nullptr;

        if(viewGrid->itemAtPosition(y,x) != nullptr){

            return qobject_cast<View*>(viewGrid->itemAtPosition(y,x)->widget());  //qt uses row-first. code does column first
        }
    }

    return nullptr;
}

QMetaObject::Connection activeCameraGrid::connectionAt(int x, int y) const
{
    if(withinBounds(x,y)){
        return conns.value(QPair<int,int>(x,y), QMetaObject::Connection());
    }
    return QMetaObject::Connection();
}

QGridLayout* activeCameraGrid::getViewGrid() const{
    return viewGrid;
}


//stops camera from playing, destroys all connections so nothing is stale
void activeCameraGrid::deActivate(int x, int y)
{
    if (!withinBounds(x, y)) return;
    const QPair<int,int> coord{x, y};

    // If a camera is on this slot, drop its destroyed-lambda too.
    if (auto* cam = activeCams.value(coord).data()) {
        if (auto c = destroyedConns.take(cam))
            QObject::disconnect(c);
    }

    activeCams.remove(coord);
    if (auto c = conns.take(coord))
        QObject::disconnect(c);
    if (auto* v = viewAt(x, y))
        v->clear();
}
