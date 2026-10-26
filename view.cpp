#include "view.h"
#include "QPainter"
#include "QRect"

View::View(QWidget* p) : QWidget(p) {}

View::~View(){
}


void View::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    if(imgData.isNull()) return;

    auto scaled = imgData.size().scaled(size(), Qt::KeepAspectRatio);
    QRect target(QPoint(0,0), scaled);
    target.moveCenter(rect().center());

    painter.drawImage(target, imgData);
}

void View::receiveFrame(const QImage& frame){
    imgData = frame;
    update();
}

void View::clear() {
    imgData = QImage();
    update();
}