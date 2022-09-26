#ifndef WAITCOUNTWGT_H
#define WAITCOUNTWGT_H

#include <QObject>
#include <QWidget>

class WaitCountWgt : public QWidget
{
    Q_OBJECT
public:
    explicit WaitCountWgt(QWidget *parent = nullptr);
private:
    void timerEvent(QTimerEvent* event);
    void showEvent(QShowEvent* event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // WAITCOUNTWGT_H
