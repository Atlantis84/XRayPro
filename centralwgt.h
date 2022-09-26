#ifndef CENTRALWGT_H
#define CENTRALWGT_H

#include <QWidget>
#include <QPixmap>
#include "opencv2/imgproc/imgproc_c.h"
class CentralWgt : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWgt(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // CENTRALWGT_H
