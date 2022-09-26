#ifndef ABOUTWGT_H
#define ABOUTWGT_H

#include <QWidget>

class AboutWgt : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
public:

signals:

public slots:
    void slot_close_about();
};

#endif // ABOUTWGT_H
