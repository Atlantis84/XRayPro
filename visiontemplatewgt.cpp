#include "visiontemplatewgt.h"
#include "title/TitleBar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "gdatafactory.h"
VisionTemplateWgt::VisionTemplateWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->setStyleSheet("QWidget{background-color:#000000}");
    resize(1152,768);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"视觉模板制作");
    pTitleBar->setFixedWidth(1152);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hAll = new QHBoxLayout();
    QVBoxLayout* vLeft = new QVBoxLayout();
    QVBoxLayout* vRight = new QVBoxLayout();
//    vAll->setMargin(0);
    vAll->addSpacing(pTitleBar->height());
    GDataFactory::get_zoom_obj()->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    GDataFactory::get_zoom_obj()->scale(0.25,0.25);
    vLeft->addWidget(GDataFactory::get_zoom_obj());
    vRight->addWidget(GDataFactory::get_template_config_wgt());
    hAll->addLayout(vLeft);
    hAll->addLayout(vRight);
    hAll->setStretch(0,2);
    hAll->setStretch(1,1);
    vAll->addLayout(hAll);
    this->setLayout(vAll);
}

void VisionTemplateWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());
}
