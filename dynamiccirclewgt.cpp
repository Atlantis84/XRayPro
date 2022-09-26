#include "dynamiccirclewgt.h"
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "gdatafactory.h"
DynamicCircleWgt::DynamicCircleWgt(QWidget *parent) : QWidget(parent)
{
    this->startTimer(50);
}

static int rotateValue = 0;
void DynamicCircleWgt::timerEvent(QTimerEvent *event)
{
    this->repaint();
    rotateValue += 10;
}

void DynamicCircleWgt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.save();
    QPoint ptCenter(400,200);
    painter.translate(ptCenter.x(),ptCenter.y());
    painter.rotate(rotateValue);
    int R = 160;
    for(int i=0;i<18;i++)
    {
        double angle = (double)(i*20)*M_PI/180.0;
        QPainterPath pathE;
        pathE.addEllipse(0-R*qCos(angle)-25,R*qSin(angle)-25,50,50);
        painter.fillPath(pathE,QBrush(QColor(0,0,255,255-i*10)));
    }
    painter.restore();

    painter.setPen(QPen(QColor(0,0,255)));
    painter.setFont(QFont(u8"Lucida Calligraphy",40,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter," X-RAY");
}
