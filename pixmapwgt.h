#ifndef PIXMAPWGT_H
#define PIXMAPWGT_H

#include <QWidget>

class PixmapWgt : public QWidget
{
    Q_OBJECT
public:
    explicit PixmapWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);

    QPixmap pix_capture;

signals:

public slots:
    void slot_receive_pixmap(QPixmap pm);
};

#endif // PIXMAPWGT_H
