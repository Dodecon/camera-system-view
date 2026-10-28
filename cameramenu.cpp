#include "cameramenu.h"
#include <QLineEdit>
#include <QLabel>
#include <QApplication>
#include <QScreen>
#include <QButtonGroup>


cameraMenu::cameraMenu(camContainer* c, QWidget *parent) : QDialog(parent), cams(c) {
    // if (!cams || cams->size() < 1) {
    //     return;
    // }
    if(cams->size() > 0){
        selectedIndex = 0;
    }

    int width = qApp->primaryScreen()->size().width();
    int height = qApp->primaryScreen()->size().height();

    resize(width / 3.5f, height / 2.5f);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* settingsLayout = new QVBoxLayout();
    QVBoxLayout* optionsLayout = new QVBoxLayout();


    QHBoxLayout* camerasLayout = new QHBoxLayout();

    QPushButton* cam1 = new QPushButton(this);
    cam1->setText("камера 1");
    cam1->setAutoDefault(false);
    cam1->setCheckable(true);
    QPushButton* cam2 = new QPushButton(this);
    cam2->setText("камера 2");
    cam2->setAutoDefault(false);
    cam2->setCheckable(true);
    QPushButton* cam3 = new QPushButton(this);
    cam3->setText("камера 3");
    cam3->setAutoDefault(false);
    cam3->setCheckable(true);
    QPushButton* cam4 = new QPushButton(this);
    cam4->setText("камера 4");
    cam4->setAutoDefault(false);
    cam4->setCheckable(true);

    QButtonGroup* camGroup = new QButtonGroup(this);
    camGroup->setExclusive(true);      // <-- only one checked at a time
    camGroup->addButton(cam1, 0);
    camGroup->addButton(cam2, 1);
    camGroup->addButton(cam3, 2);
    camGroup->addButton(cam4, 3);

    camerasLayout->addWidget(cam1);
    camerasLayout->addWidget(cam2);
    camerasLayout->addWidget(cam3);
    camerasLayout->addWidget(cam4);
    camerasLayout->setAlignment(Qt::AlignLeft);
    camerasLayout->setSpacing(0);




    QLabel* topHint = new QLabel("Настройки подключения rtsp", this);
    topHint->setAlignment(Qt::AlignCenter);
    topHint->setMargin(5);




    QVBoxLayout* VTextLayout = new QVBoxLayout();

    QLabel* currentLink = new QLabel("Ссылка сейчас:", this); //port
    currentLink->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(currentLink);

    QLabel* rtsp = new QLabel("RTSP-поток:", this); //port
    rtsp->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(rtsp);

    QLabel* login = new QLabel("Логин:", this);
    login->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(login);

    QLabel* password = new QLabel("Пароль:", this);
    password->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(password);

    QLabel* ip = new QLabel("IP:", this);
    ip->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(ip);

    QLabel* stream = new QLabel("Имя потока:", this);
    stream->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(stream);

    QLabel* customLink = new QLabel("Своя ссылка:", this);
    customLink->setAlignment(Qt::AlignRight);
    VTextLayout->addWidget(customLink);



    QVBoxLayout* VFieldLayout = new QVBoxLayout();

    CurrentLink = new QLineEdit(this);
    CurrentLink->setReadOnly(true);
    VFieldLayout->addWidget(CurrentLink);

    Port = new QLineEdit(this);
    Port->setAcceptDrops(0);
    VFieldLayout->addWidget(Port);


    Login= new QLineEdit(this);
    VFieldLayout->addWidget(Login);


    Password = new QLineEdit(this);
    VFieldLayout->addWidget(Password);

    Ip = new QLineEdit(this);
    VFieldLayout->addWidget(Ip);


    Stream = new QLineEdit(this);
    VFieldLayout->addWidget(Stream);

    CustomLink = new QLineEdit(this);
    VFieldLayout->addWidget(CustomLink);



    QHBoxLayout* columnsLayout = new QHBoxLayout();
    columnsLayout->addLayout(VTextLayout);
    columnsLayout->addLayout(VFieldLayout);




    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* saveButton = new QPushButton(this);
    saveButton->setText("Сохранить");
    QPushButton* closeButton = new QPushButton(this);
    closeButton->setText("выйти");
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(closeButton);



    optionsLayout->addWidget(topHint);
    optionsLayout->addStretch(1);
    optionsLayout->addLayout(camerasLayout);
    optionsLayout->addLayout(columnsLayout);
    optionsLayout->addStretch(3);
    optionsLayout->addLayout(buttonLayout);

    QPushButton* btn1 = new QPushButton(this);
    btn1->setText("----");
    btn1->setAutoDefault(false);
    settingsLayout->addWidget(btn1);
    QPushButton* btn2 = new QPushButton(this);
    btn2->setText("----");
    btn2->setAutoDefault(false);
    settingsLayout->addWidget(btn2);
    QPushButton* btn3 = new QPushButton(this);
    btn3->setText("----");
    btn3->setAutoDefault(false);
    settingsLayout->addWidget(btn3);
    QPushButton* btn4 = new QPushButton(this);
    btn4->setText("----");
    btn4->setAutoDefault(false);
    settingsLayout->addWidget(btn4);

    mainLayout->addLayout(settingsLayout, 1);
    mainLayout->addLayout(optionsLayout, 3);







    connect(camGroup, &QButtonGroup::idClicked, this, [camGroup, this](int id) {
        clearInputFields();

        selectedIndex = id;
        getURLforUI(id);

        camGroup->button(id)->setChecked(true);
    });




    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        saveURL(selectedIndex);
    });


    cam1->setChecked(true);
    getURLforUI(0);

}

cameraMenu::~cameraMenu() {

}


void cameraMenu::clearInputFields(){

    Login->setText("");
    Password->setText("");
    Ip->setText("");
    Port->setText("");
    Stream->setText("");
}


void cameraMenu::getURLforUI(int index){


    baseSecurityCamera* selectedCam = cams->at(index);


    QUrl camURL = selectedCam->getUrl();

    CurrentLink->setText(camURL.toString());

    if(QString::number(camURL.port()) == "-1" || camURL.isEmpty()){
        Login->setText("");
        Password->setText("");
        Ip->setText("");
        Port->setText("");
        Stream->setText("");
    }

    else if(camURL.isValid() && !camURL.isEmpty()){
        Login->setText(camURL.userName());
        Password->setText(camURL.password());
        Ip->setText(camURL.host());
        Port->setText(QString::number(camURL.port()));
        Stream->setText(camURL.path().remove(0,1));
    }
}


void cameraMenu::saveURL(int index){

    if(selectedIndex== -1){
        return;
    }

    QString newURL = QString(CustomLink->text());


    cams->at(selectedIndex)->setUrl(newURL);
    CurrentLink->setText(newURL);

}
