#include "centralwgt.h"
#include <QPainter>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QVBoxLayout>
#include "zoommoveobj.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"
using namespace std;

CentralWgt::CentralWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hAll = new QHBoxLayout();

    QGroupBox* grpBoxPixmap = new QGroupBox(u8"过程图像");
    QGroupBox* grpBoxInfo = new QGroupBox(u8"过程信息");
    grpBoxPixmap->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");
    grpBoxInfo->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");
    QVBoxLayout* vLeft = new QVBoxLayout();
    QVBoxLayout* vRight = new QVBoxLayout();
    vLeft->addWidget(GDataFactory::get_process_pixmap_wgt());
    vRight->addWidget(GDataFactory::get_process_info_wgt());
    grpBoxPixmap->setLayout(vLeft);
    grpBoxInfo->setLayout(vRight);
    hAll->addWidget(grpBoxPixmap);
    hAll->addWidget(grpBoxInfo);
    hAll->setStretch(0,1);
    hAll->setStretch(1,1);
    vAll->addLayout(hAll);
    this->setLayout(vAll);
}


void CentralWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());
//    QPixmap pm;
//    pm.load("f:\\1.tif");

//    painter.drawPixmap(0,0,this->height(),this->height(),create_pixmap_from_raw());
}
