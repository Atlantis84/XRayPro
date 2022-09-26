#ifndef DYNAMICCIRCLEWGT_H
#define DYNAMICCIRCLEWGT_H

#include <QObject>
#include <QWidget>
#include <QThread>
class DynamicCircleWgt : public QWidget
{
    Q_OBJECT
public:
    explicit DynamicCircleWgt(QWidget *parent = nullptr);

private:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};


#endif // DYNAMICCIRCLEWGT_H
