#include "processpixmapwgt.h"
#include <QPainter>
#include "gdatafactory.h"
ProcessPixmapWgt::ProcessPixmapWgt(QWidget *parent) : QWidget(parent)
{
    m_pPixmap.load("F:\\1.tif");
}

void ProcessPixmapWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());
    painter.drawPixmap(this->rect(),m_pPixmap);
}

void ProcessPixmapWgt::slot_receive_pixmap(QPixmap pm)
{
    if(GDataFactory::get_factory()->get_system_status() != Run_Status)
        return;
    m_pPixmap = pm;
    this->repaint();
}
