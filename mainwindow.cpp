#include "mainwindow.h"
#include "securitycamera.h"
#include "cameramenu.h"
#include "QDir"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>

#include "logListner.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget* centralWidget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    camGrid = new QGridLayout();
    camGrid->setSpacing(4);

    video1 = new QVideoWidget(this);
    QSizePolicy sp1 = video1->sizePolicy();
    sp1.setRetainSizeWhenHidden(true);
    video1->setSizePolicy(sp1);

    video2 = new QVideoWidget(this);
    QSizePolicy sp2 = video2->sizePolicy();
    sp2.setRetainSizeWhenHidden(true);
    video2->setSizePolicy(sp2);

    video3 = new QVideoWidget(this);
    QSizePolicy sp3 = video3->sizePolicy();
    sp3.setRetainSizeWhenHidden(true);
    video3->setSizePolicy(sp3);

    video4 = new QVideoWidget(this);
    QSizePolicy sp4 = video4->sizePolicy();
    sp4.setRetainSizeWhenHidden(true);
    video4->setSizePolicy(sp4);


    camGrid->addWidget(video1, 0, 0);
    camGrid->addWidget(video2, 0, 1);
    camGrid->addWidget(video3, 1, 0);
    camGrid->addWidget(video4, 1, 1);



    fallbackGrid = new QGridLayout();
    fallbackGrid->setSpacing(4);

    fallback1 = new QLabel(this);
    fallback1->setPixmap(QPixmap(fallbackUrl));
    fallback1->setScaledContents(true);


    fallback2 = new QLabel(this);
    fallback2->setPixmap(QPixmap(fallbackUrl));
    fallback2->setScaledContents(true);


    fallback3 = new QLabel(this);
    fallback3->setPixmap(QPixmap(fallbackUrl));
    fallback3->setScaledContents(true);

    fallback4 = new QLabel(this);
    fallback4->setPixmap(QPixmap(fallbackUrl));
    fallback4->setScaledContents(true);

    fallbackGrid->addWidget(fallback1, 0, 0);
    fallbackGrid->addWidget(fallback2, 0, 1);
    fallbackGrid->addWidget(fallback3, 1, 0);
    fallbackGrid->addWidget(fallback4, 1, 1);



    QGridLayout* overlayLayout = new QGridLayout();
    overlayLayout->setContentsMargins(0,0,0,0);
    overlayLayout->setSpacing(0);

    overlayLayout->addLayout(camGrid, 0, 0);

    overlayLayout->addLayout(fallbackGrid, 0, 0);

    bigVideo = new QVideoWidget(this);
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

    auto labelSink = std::make_shared<logListner_mt>(label);

    std::string logPath = std::string(PROJECT_ROOT_DIR) + "/resources/logs.txt";
    QDir().mkpath(QString::fromStdString(std::string(PROJECT_ROOT_DIR) + "/resources"));

    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        logPath,
        1024 * 1024 * 5,
        10
        );


    std::vector<spdlog::sink_ptr> sinks = { fileSink, labelSink };
    auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

    spdlog::set_default_logger(logger);


    spdlog::info("application started");

    this->showFullScreen();
}



MainWindow::~MainWindow()
{
}


QVideoWidget* MainWindow::getSmallWidgetAt(int row, int col) {
    for (int i = 0; i < camGrid->count(); i++) {
        QLayoutItem* item = camGrid->itemAt(i);
        QWidget* widget = item->widget();

        if (widget == bigVideo) continue;

        int r, c, rowSpan, colSpan;
        camGrid->getItemPosition(i, &r, &c, &rowSpan, &colSpan);

        if (r == row && c == col) {
            return qobject_cast<QVideoWidget*>(widget);
        }
    }
    return nullptr;
}


void MainWindow::showEvent(QShowEvent *event){
    QMainWindow::showEvent(event);

    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    QString highResUrl = QString("rtsp://admin:admin@10.0.2.36:554/main");
    QUrl localUrl = QUrl("rtsp://admin:admin@10.0.2.36:554/sub");
    QUrl none = QUrl("qwerty");


    connect(btn1, &QPushButton::clicked, this, [this](){onViewCameraPressed(QPair<int,int>(0,0));});
    connect(btn2, &QPushButton::clicked, this, [this](){onViewCameraPressed(QPair<int,int>(0,1));});
    connect(btn3, &QPushButton::clicked, this, [this](){onViewCameraPressed(QPair<int,int>(1,0));});
    connect(btn4, &QPushButton::clicked, this, [this](){onViewCameraPressed(QPair<int,int>(1,1));});

    connect(btn5, &QPushButton::clicked, this, &MainWindow::onViewExit);


    cams = new camContainer(this);

    cams->addCamera(highResUrl, camGrid);
    cams->addCamera(localUrl, camGrid);
    cams->addCamera(none, camGrid);
    cams->addCamera(none, camGrid);

    cams->schedule(cams->at(0));
    cams->schedule(cams->at(1));
    cams->schedule(cams->at(2));
    cams->schedule(cams->at(3));


    connect(btnMenu, &QPushButton::clicked, this, &MainWindow::onMenuButtonPressed);
}



void MainWindow::onViewCameraPressed(QPair<int,int> coords){
    int row = coords.first;
    int col = coords.second;


    QVideoWidget* targetSmallWidget = getSmallWidgetAt(row, col);
    if (!targetSmallWidget) return;


    securityCamera* targetCam = nullptr;

    for (int i = 0; i < cams->size(); ++i) {
        if (cams->at(i)->getVideoWidget() == targetSmallWidget) {
            targetCam = cams->at(i);
            break;
        }
    }

    if (!targetCam) return;

    if (bigCamera == targetCam) return;

    if (bigCamera != nullptr) {
        bigCamera->setVideoOutput(savedSmallWidget);
    }

    savedSmallWidget = targetSmallWidget;

    targetCam->setVideoOutput(bigVideo);

    bigVideo->show();
    bigCamera = targetCam;
}


void MainWindow::onViewExit(){
    if(bigCamera == nullptr) return;

    bigCamera->setVideoOutput(savedSmallWidget);

    bigVideo->hide();
    bigCamera = nullptr;
    savedSmallWidget = nullptr;

}


void MainWindow::onMenuButtonPressed(){
    cameraMenu menu(cams, this);
    menu.exec();
}
