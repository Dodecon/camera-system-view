#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QVideoWidget>
#include <QLabel>
#include <QObject>

class cameraView : public QObject
{
    Q_OBJECT
private:
    QVideoWidget* cameraOutputWidget;
    QLabel* fallbackWidget;

public:
    explicit cameraView(QVideoWidget* camW, QLabel* fallB);
    ~cameraView();

    QVideoWidget* getCameraOutputWidget() const;
    void setCameraOutputWidget(QVideoWidget *newCameraOutputWidget);

    QLabel* getFallbackWidget() const   ;
    void setFallbackWidget(QLabel* newFallbackWidget);

private slots:
    void onVideoFeedRecieved(bool isRecieved);
};

#endif // CAMERAVIEW_H
