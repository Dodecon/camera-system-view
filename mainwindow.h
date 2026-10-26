#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include <QTimer>
#include <QVideoWidget>
#include <QUrl>
#include <QPushButton>
#include <QPair>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QPointer>
#include "securitycamera.h"
#include "camcontainer.h"
#include "cameramenu.h"

// class camContainer;                  // only if you need the pointer type
// #include "securitycamera.h"
// #include "camcontainer.h"
// #include "logListner.h"

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

    // camContainer* cams = nullptr;      // commented until you re-add it

    QGridLayout* camGrid;

    camContainer* cameras;

    activeCameraGrid* mainCamGrid;
    QPointer<QGridLayout> mainOutputLayout;

    activeCameraGrid* bigVideoView;

    cameraMenu* CamSettingsMenu;

    QPushButton* btnMenu;
    QLabel* label;
    QPushButton* btn1;
    QPushButton* btn2;
    QPushButton* btn3;
    QPushButton* btn4;
    QPushButton* btn5;

private slots:

    void onShowFullScreenButtonPressed(QPair<int, int> showAtPos);
    void onCloseFullScreenButtonPressed();
};
#endif // MAINWINDOW_H