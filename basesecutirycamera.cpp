#include "Basesecutirycamera.h"


baseSecurityCamera::baseSecurityCamera(QObject* p, const QUrl& URL) : QObject(p), url(URL) {};


baseSecurityCamera::camState baseSecurityCamera::getState() const{
    return this->state;
}

QUrl baseSecurityCamera::getUrl() const{
    return this->url;
}


void baseSecurityCamera::setState(const CamState s){
    this->state = s;

    emit stateChanged(this->state);
}

void baseSecurityCamera::setUrl(const QUrl& u){
    this->url = u;
}