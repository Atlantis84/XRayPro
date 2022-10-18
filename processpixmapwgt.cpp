#include "processpixmapwgt.h"
#include <QPainter>
#include "gdatafactory.h"
#include "QsLog.h"
ProcessPixmapWgt::ProcessPixmapWgt(QWidget *parent) : QWidget(parent)
{
    m_pPixmap.load("F:\\1.tif");
}

void ProcessPixmapWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(0,0,0)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());

    painter.setPen(QPen(QColor(255,255,255,100),5));
    painter.setFont(QFont(u8"KaiTi",50,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"¹ý³ÌÍ¼ÏñÏÔÊ¾");

    if(GDataFactory::get_factory()->get_system_status() != Run_Status)
        return;

    QString iSign = GDataFactory::get_factory()->get_config_para("IMAGE_STRETCH_FILL");
    if(iSign == "0")
    {
        QRect rtSquare;
        rtSquare.setLeft((this->rect().width()-this->rect().height())/2);
        rtSquare.setTop(0);
        rtSquare.setWidth(this->rect().height());
        rtSquare.setHeight(this->rect().height());

        QRect rtUSquare;
        float ratio = (float)((float)(m_pPixmap.height())/(float)(m_pPixmap.width()));
        int ht = (float)(this->rect().width())*ratio;
        rtUSquare.setLeft(0);
        rtUSquare.setTop((this->rect().height()-ht)/2);
        rtUSquare.setWidth(this->rect().width());
        rtUSquare.setHeight(ht);

        if(m_pPixmap.width() == m_pPixmap.height())
            painter.drawPixmap(rtSquare,m_pPixmap);
        else {
            painter.drawPixmap(rtUSquare,m_pPixmap);
        }
    }
    else
        painter.drawPixmap(this->rect(),m_pPixmap);

}

void ProcessPixmapWgt::slot_receive_pixmap(QPixmap pm)
{
    if(GDataFactory::get_factory()->get_system_status() != Run_Status)
        return;
    m_pPixmap = pm;
    this->repaint();
}
