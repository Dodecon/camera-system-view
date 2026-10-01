#include "cameraview.h"


cameraView::cameraView(QVideoWidget* camW, QLabel* fallB) : cameraOutputWidget(camW), fallbackWidget(fallB) {
}

cameraView::~cameraView(){

}


QVideoWidget* cameraView::getCameraOutputWidget() const {
    return cameraOutputWidget;
}

void cameraView::setCameraOutputWidget(QVideoWidget *newCameraOutputWidget){
    cameraOutputWidget = newCameraOutputWidget;
}




QLabel* cameraView::getFallbackWidget() const {
    return fallbackWidget;
}

void cameraView::setFallbackWidget(QLabel *newFallbackWidget){
    fallbackWidget = newFallbackWidget;
}



void cameraView::onVideoFeedRecieved(bool isRecieved){
    if(cameraOutputWidget == nullptr){return;}


    if(!isRecieved){
        cameraOutputWidget->hide();
    }
    else{
        cameraOutputWidget->show();
    }
}
