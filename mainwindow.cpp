#include "mainwindow.h"
#include "securitycamera.h"
#include "camcontainer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QGridLayout>
#include <QPushButton>
#include "cameraview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // camGrid = new QGridLayout();
    // camGrid->setSpacing(4);


    cam1 = new securityCamera(this, QUrl("C:/Users/Irbis/Downloads/shaman-ya-russkiy_580621.mp4"));
    cam2 = new securityCamera(this, QUrl("C:/Users/Irbis/Downloads/shaman-ya-russkiy_580621.mp4"));
    cam3 = new securityCamera(this, QUrl("C:/Users/Irbis/Downloads/shaman-ya-russkiy_580621.mp4"));
    cam4 = new securityCamera(this, QUrl("C:/Users/Irbis/Downloads/shaman-ya-russkiy_580621.mp4"));


    // video1 = new cameraView(this);
    // QSizePolicy sp1 = video1->sizePolicy();
    // sp1.setRetainSizeWhenHidden(true);
    // video1->setSizePolicy(sp1);

    // video2 = new cameraView(this);
    // QSizePolicy sp2 = video2->sizePolicy();
    // sp2.setRetainSizeWhenHidden(true);
    // video2->setSizePolicy(sp2);

    // video3 = new cameraView(this);
    // QSizePolicy sp3 = video3->sizePolicy();
    // sp3.setRetainSizeWhenHidden(true);
    // video3->setSizePolicy(sp3);

    // video4 = new cameraView(this);
    // QSizePolicy sp4 = video4->sizePolicy();
    // sp4.setRetainSizeWhenHidden(true);
    // video4->setSizePolicy(sp4);

    // camGrid->addWidget(video1, 0, 0);
    // camGrid->addWidget(video2, 0, 1);
    // camGrid->addWidget(video3, 1, 0);
    // camGrid->addWidget(video4, 1, 1);

    mainOutputLayout = new QGridLayout(this);
    mainOutputLayout->setSpacing(4);

    mainCamGrid = new activeCameraGrid(this, 2, 2, mainOutputLayout);
    mainCamGrid->setActive(cam1, 0, 0);
    mainCamGrid->setActive(cam2, 1, 0);
    mainCamGrid->setActive(cam3, 0, 1);
    mainCamGrid->setActive(cam4, 1, 1);


    QGridLayout* overlayLayout = new QGridLayout();
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->setSpacing(0);

    overlayLayout->addLayout(mainCamGrid->getViewGrid(), 0, 0);

    bigVideo = new cameraView(this);
    overlayLayout->addWidget(bigVideo, 0, 0);
    bigVideo->hide();

    mainLayout->addLayout(overlayLayout, 6);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(10, 10, 10, 10);
    buttonLayout->setSpacing(10);

    QSizePolicy expandPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    btnMenu = new QPushButton("Menu", this);
    btnMenu->setMinimumSize(10, 10);
    btnMenu->setSizePolicy(expandPolicy);

    label = new QLabel("TextLabel", this);
    label->setSizePolicy(expandPolicy);
    label->setAlignment(Qt::AlignCenter);

    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QSizePolicy rectangularPolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    btn1 = new QPushButton("1", this);
    btn1->setMinimumSize(10, 10);
    btn1->setSizePolicy(rectangularPolicy);

    btn2 = new QPushButton("2", this);
    btn2->setMinimumSize(10, 10);
    btn2->setSizePolicy(rectangularPolicy);

    btn3 = new QPushButton("3", this);
    btn3->setMinimumSize(10, 10);
    btn3->setSizePolicy(rectangularPolicy);

    btn4 = new QPushButton("4", this);
    btn4->setMinimumSize(10, 10);
    btn4->setSizePolicy(rectangularPolicy);

    btn5 = new QPushButton("5", this);
    btn5->setMinimumSize(10, 10);
    btn5->setSizePolicy(rectangularPolicy);

    buttonLayout->addWidget(btnMenu);
    buttonLayout->addWidget(label);
    buttonLayout->addSpacerItem(spacer);
    buttonLayout->addWidget(btn1);
    buttonLayout->addWidget(btn2);
    buttonLayout->addWidget(btn3);
    buttonLayout->addWidget(btn4);
    buttonLayout->addWidget(btn5);

    mainLayout->addLayout(buttonLayout, 1);

    setCentralWidget(centralWidget);
    this->showFullScreen();
}

MainWindow::~MainWindow()
{
}

QVideoWidget* MainWindow::getSmallWidgetAt(int row, int col)
{
    return nullptr;   // stub for now
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    cam1->play();
    cam2->play();
    cam3->play();
    cam4->play();

}

void MainWindow::onViewExit()
{
    bigVideo->hide();
    savedSmallWidget = nullptr;
}

void MainWindow::onMenuButtonPressed()
{
    // cameraMenu menu(nullptr, this);
    // menu.exec();
}