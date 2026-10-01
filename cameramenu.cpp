#include "cameramenu.h"
#include <QLineEdit>
#include <QLabel>

cameraMenu::cameraMenu(camContainer* c, QWidget *parent) : QDialog(parent), cams(c) {
    if (!cams || cams->size() < 1) {
        return;
    }
    if(cams->size() > 0){
        selectedIndex = 0;
    }


    resize(450, 300);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* settingsLayout = new QVBoxLayout();
    QVBoxLayout* optionsLayout = new QVBoxLayout();


    QHBoxLayout* camerasLayout = new QHBoxLayout();

    QPushButton* cam1 = new QPushButton(this);
    cam1->setText("камера 1");
    QPushButton* cam2 = new QPushButton(this);
    cam2->setText("камера 2");
    QPushButton* cam3 = new QPushButton(this);
    cam3->setText("камера 3");
    QPushButton* cam4 = new QPushButton(this);
    cam4->setText("камера 4");

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

    QLabel* rtsp = new QLabel("RTSP-поток:", this);
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



    QVBoxLayout* VFieldLayout = new QVBoxLayout();

    Port = new QLineEdit(this);
    VFieldLayout->addWidget(Port);


    Login= new QLineEdit(this);
    VFieldLayout->addWidget(Login);


    Password = new QLineEdit(this);
    VFieldLayout->addWidget(Password);

    Ip = new QLineEdit(this);
    VFieldLayout->addWidget(Ip);


    Stream = new QLineEdit(this);
    VFieldLayout->addWidget(Stream);



    QHBoxLayout* columnsLayout = new QHBoxLayout();
    columnsLayout->addLayout(VTextLayout);
    columnsLayout->addLayout(VFieldLayout);




    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* saveButton = new QPushButton(this);
    saveButton->setText("Сохранить и выйти");
    QPushButton* closeButton = new QPushButton(this);
    closeButton->setText("выйти без сохранения");
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(closeButton);



    optionsLayout->addWidget(topHint);
    optionsLayout->addStretch(1);
    optionsLayout->addLayout(camerasLayout);
    optionsLayout->addLayout(columnsLayout);
    optionsLayout->addStretch(3);
    optionsLayout->addLayout(buttonLayout);

    QPushButton* btn1 = new QPushButton(this);
    btn1->setText("меню");
    settingsLayout->addWidget(btn1);
    QPushButton* btn2 = new QPushButton(this);
    btn2->setText("меню");
    settingsLayout->addWidget(btn2);
    QPushButton* btn3 = new QPushButton(this);
    btn3->setText("меню");
    settingsLayout->addWidget(btn3);
    QPushButton* btn4 = new QPushButton(this);
    btn4->setText("меню");
    settingsLayout->addWidget(btn4);

    mainLayout->addLayout(settingsLayout, 1);
    mainLayout->addLayout(optionsLayout, 3);







    connect(cam1, &QPushButton::clicked, this, [this](){
        getURLforUI(0);
    });
    connect(cam2, &QPushButton::clicked, this, [this](){
        getURLforUI(1);
    });
    connect(cam3, &QPushButton::clicked, this, [this](){
        getURLforUI(2);
    });
    connect(cam4, &QPushButton::clicked, this, [this](){
        getURLforUI(3);
    });




    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        saveURL();
    });



    getURLforUI(0);

}

cameraMenu::~cameraMenu() {

}




void cameraMenu::getURLforUI(int index){
    if (selectedIndex == -1) return;


    securityCamera* selectedCam = cams->at(index);
    selectedIndex = index;


    QUrl camURL = selectedCam->getUrl();

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


void cameraMenu::saveURL(){

    if(Port->text().isEmpty() || Login->text().isEmpty() || Password->text().isEmpty() || Ip->text().isEmpty() || Stream->text().isEmpty() || selectedIndex== -1){
        return;
    }

    QString newURL = QString("rtsp://" + Login->text() + ":" + Password->text() + "@" + Ip->text() + ":" + Port->text() + "/" + Stream->text());


    cams->at(selectedIndex)->setUrl(newURL);
    cams->schedule(cams->at(selectedIndex));

}
