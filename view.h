#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QWidget>
#include <QImage>

class View : public QWidget
{
    Q_OBJECT
private:
    QImage imgData;

    void paintEvent(QPaintEvent* event) override;

public:

    explicit View(QWidget* p = nullptr);
    ~View();


    void clear();
public slots:
    void receiveFrame(const QImage& frame);
};

#endif // VIEW_H
