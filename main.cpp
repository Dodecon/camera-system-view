#include "mainwindow.h"
#include <QApplication>

extern "C" {
#include <libavformat/avformat.h>
}

int main(int argc, char *argv[])
{
    avformat_network_init();

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
