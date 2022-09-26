#include "welcomedlg.h"
#include <QPainter>
#include "gdatafactory.h"
#include <QtMath>
#include <QVBoxLayout>
WelcomeDlg::WelcomeDlg()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground,true);
    this->resize(800,600);

//    this->startTimer(50);


    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addWidget(GDataFactory::get_dynamic_circle_wgt());
    vAll->addWidget(GDataFactory::get_equip_info_wgt());
    vAll->setStretch(0,3);
    vAll->setStretch(1,1);
    this->setLayout(vAll);

}

void WelcomeDlg::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

void WelcomeDlg::set_text(int eqID, QString info)
{
}

void WelcomeDlg::timerEvent(QTimerEvent *event)
{
    this->repaint();
}

void WelcomeDlg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.save();
//    QPoint ptCenter(400,200);
//    painter.translate(ptCenter.x(),ptCenter.y());
//    painter.rotate(rotateValue);
//    int R = 160;
//    for(int i=0;i<18;i++)
//    {
//        double angle = (double)(i*20)*M_PI/180.0;
//        QPainterPath pathE;
//        pathE.addEllipse(0-R*qCos(angle)-25,R*qSin(angle)-25,50,50);
//        painter.fillPath(pathE,QBrush(QColor(0,0,255,255-i*10)));
//    }
//    painter.restore();
}

