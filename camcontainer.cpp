#include "camcontainer.h"


camContainer::camContainer(QObject *parent) : QObject(parent){

}

camContainer::~camContainer(){

}




void camContainer::addCamera(QUrl url, QGridLayout* layout){
    securityCamera* newCam = new securityCamera(url, this);
    CameraList.append(newCam);
}


void camContainer::schedule(securityCamera* cam){
    if(cam == nullptr || camsToPlay.contains(cam)){return;}


    camsToPlay.emplaceBack(cam);

    if(camsToPlay.size() == 1){
        startNextCam();
    }
}



void camContainer::startNextCam(){
    if(camsToPlay.isEmpty()){
        return;
    }

    securityCamera* cam = camsToPlay.front();

    connect(cam, &securityCamera::connectionAttempted,
            this, &camContainer::onCameraReady);

    cam->play();
}


void camContainer::onCameraReady(){

    securityCamera* cam = camsToPlay.dequeue();
    disconnect(cam, &securityCamera::connectionAttempted, this, &camContainer::onCameraReady);

    if(!camsToPlay.isEmpty()){
        startNextCam();
    }
}





securityCamera* camContainer::at(int index){
    if(index >= 0 && index < CameraList.size()){
        return CameraList.at(index);
    }
    return nullptr;
}



void camContainer::append(securityCamera* cam){
    CameraList.append(cam);
}


void camContainer::remove(int index){
    if(index >= 0 && index < CameraList.size() && !CameraList.isEmpty()){
        delete CameraList.takeAt(index);
    }
}
void camContainer::remove(){
    if(!CameraList.isEmpty()){
        delete CameraList.takeAt(CameraList.size()-1);
    }

}

int camContainer::size(){
    return CameraList.size();
}
