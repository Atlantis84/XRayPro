#include "waitcountwgt.h"
#include "QsLog.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
WaitCountWgt::WaitCountWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() |Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    this->resize(800,600);
    this->startTimer(50);
}

static int rotateValue = 0;
void WaitCountWgt::timerEvent(QTimerEvent *event)
{
    this->repaint();
    rotateValue += 10;
}

void WaitCountWgt::showEvent(QShowEvent *event)
{
}

void WaitCountWgt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.save();
    QPoint ptCenter(400,300);
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
    painter.setFont(QFont(u8"Lucida Calligraphy",28,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter,"WAITING");
}
