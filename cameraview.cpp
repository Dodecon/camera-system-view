#include "cameraview.h"
#include "QPainter"
#include "QRect"

cameraView::cameraView(QWidget* p) : QWidget(p) {}

cameraView::~cameraView(){
}


void cameraView::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    if(imgData.isNull()) return;

    auto scaled = imgData.size().scaled(size(), Qt::KeepAspectRatio);
    QRect target(QPoint(0,0), scaled);
    target.moveCenter(rect().center());

    painter.drawImage(target, imgData);
}

void cameraView::receiveFrame(const QImage& frame){
    imgData = frame;
    update();
}

void cameraView::clear() {
    imgData = QImage();
    update();
}