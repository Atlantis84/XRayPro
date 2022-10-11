#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void set_count_total(){DelayValue-=5;this->repaint();}
private:

    int DelayValue;

signals:

public slots:
};

#endif // LEFTWIDGET_H
