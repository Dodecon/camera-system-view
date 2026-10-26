#include "mainwindow.h"
#include "securitycamera.h"
#include "camcontainer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QGridLayout>
#include <QPushButton>
#include "activecameragrid.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);



    cameras = new camContainer(this);

    cameras->addCamera<securityCamera>(QUrl("C:/Users/Irbis/Downloads/shaman-ya-russkiy_580621.mp4"));
    cameras->addCamera<securityCamera>(QUrl("https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8"));
    cameras->addCamera<securityCamera>(QUrl("http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8"));
    cameras->addCamera<securityCamera>(QUrl("rtsp://admin:123456@enster.oicp.net/stream0"));


    mainOutputLayout = new QGridLayout(this);
    mainOutputLayout->setSpacing(4);

    mainCamGrid = new activeCameraGrid(this, 2, 2, mainOutputLayout);
    mainCamGrid->setActive(cameras->at(0), 0, 0);
    mainCamGrid->setActive(cameras->at(1), 1, 0);
    mainCamGrid->setActive(cameras->at(2), 0, 1);
    mainCamGrid->setActive(cameras->at(3), 1, 1);


    QGridLayout* overlayLayout = new QGridLayout();
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->setSpacing(0);

    overlayLayout->addLayout(mainCamGrid->getViewGrid(), 0, 0);

    bigVideoView = new activeCameraGrid(this, 1, 1);
    overlayLayout->addLayout(bigVideoView->getViewGrid(), 0, 0);
    bigVideoView->viewAt(0,0)->hide();

    mainLayout->addLayout(overlayLayout, 6);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(10, 10, 10, 10);
    buttonLayout->setSpacing(4);

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

    connect(btn1, &QPushButton::clicked, this, [this]() {onShowFullScreenButtonPressed(QPair<int, int>(0, 0));} );
    connect(btn2, &QPushButton::clicked, this, [this]() {onShowFullScreenButtonPressed(QPair<int, int>(1, 0));} );
    connect(btn3, &QPushButton::clicked, this, [this]() {onShowFullScreenButtonPressed(QPair<int, int>(0, 1));} );
    connect(btn4, &QPushButton::clicked, this, [this]() {onShowFullScreenButtonPressed(QPair<int, int>(1, 1));} );
    connect(btn5, &QPushButton::clicked, this, &MainWindow::onCloseFullScreenButtonPressed);


    CamSettingsMenu = new cameraMenu(cameras, this); // a menu to edit a cameras settings

    connect(btnMenu, &QPushButton::clicked, CamSettingsMenu, &QDialog::show);

    setCentralWidget(centralWidget);
    this->showFullScreen();
}

MainWindow::~MainWindow()
{
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    mainCamGrid->activeCameraAt(0,0)->play();
    mainCamGrid->activeCameraAt(1,0)->play();
    mainCamGrid->activeCameraAt(0,1)->play();
    mainCamGrid->activeCameraAt(1,1)->play();
}



void MainWindow::onShowFullScreenButtonPressed(QPair<int, int> showAtPos){
    bigVideoView->setActive(mainCamGrid->activeCameraAt(showAtPos.first, showAtPos.second), 0, 0);

    bigVideoView->viewAt(0,0)->show();
}

void MainWindow::onCloseFullScreenButtonPressed(){
    bigVideoView->deActivate(0,0);

    bigVideoView->viewAt(0,0)->hide();
}