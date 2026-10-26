#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "camcontainer.h"

class cameraMenu : public QDialog // We inherit from QDialog for pop-up windows
{
    Q_OBJECT

public:
    explicit cameraMenu(camContainer* c, QWidget *parent = nullptr);
    ~cameraMenu();

private:
    camContainer* cams;
    int selectedIndex = -1;

    QLineEdit *Port;
    QLineEdit *Login;
    QLineEdit *Password;
    QLineEdit *Ip;
    QLineEdit *Stream;
    QLineEdit* CustomLink;

    void saveURL(int index);
    void getURLforUI(int index);
};

#endif // MENUDIALOG_H
