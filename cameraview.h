#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QObject>
#include <QWidget>
#include <QImage>

class cameraView : public QWidget
{
    Q_OBJECT
private:
    QImage imgData;

    void paintEvent(QPaintEvent* event) override;

public:

    explicit cameraView(QWidget* p = nullptr);
    ~cameraView();


    void clear();
public slots:
    void receiveFrame(const QImage& frame);
};

#endif // CAMERAVIEW_H
