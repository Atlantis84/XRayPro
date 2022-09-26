#include "pixmapwgt.h"
#include <QPainter>
PixmapWgt::PixmapWgt(QWidget *parent) : QWidget(parent)
{
    pix_capture.load(":/icon/CIA_Origin.png");
}

void PixmapWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(this->rect(),pix_capture);
}

void PixmapWgt::slot_receive_pixmap(QPixmap pm)
{
    pix_capture = pm;
    this->repaint();
}
