#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include <QTimer>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QPushButton>
#include <QPair>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

#include "securitycamera.h"
#include "camcontainer.h"
#include "logListner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent* event) override;

private:
    inline static const QString fallbackUrl = ":/resources/no-connection.jpeg";

    camContainer* cams;

    QGridLayout* camGrid;
    QGridLayout* fallbackGrid;

    QVideoWidget* video1;
    QVideoWidget* video2;
    QVideoWidget* video3;
    QVideoWidget* video4;

    QLabel* fallback1;
    QLabel* fallback2;
    QLabel* fallback3;
    QLabel* fallback4;

    QVideoWidget* bigVideo;


    securityCamera* bigCamera = nullptr;
    QVideoWidget* savedSmallWidget = nullptr;

    QPushButton* btnMenu;
    QLabel* label;
    QPushButton* btn1;
    QPushButton* btn2;
    QPushButton* btn3;
    QPushButton* btn4;
    QPushButton* btn5;


    QVideoWidget* getSmallWidgetAt(int row, int col);


private slots:
    void onViewCameraPressed(QPair<int,int> coords);
    void onViewExit();

    void onMenuButtonPressed();
};
#endif // MAINWINDOW_H
