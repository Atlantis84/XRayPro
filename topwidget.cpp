#include "topwidget.h"
#include <QIcon>
#include "title/TitleBar.h"
#include <QVBoxLayout>
#include "gdatafactory.h"
#include <QPainter>
#include <QCloseEvent>
TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->setStyleSheet("QWidget{background-color:#000000}");
    resize(1800,1000);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"X-RAYµãÁÏ»ú");
    pTitleBar->setFixedWidth(1800);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    vAll->setSpacing(0);
    vAll->setMargin(0);
    GDataFactory::get_main_window()->setParent(this);
    vAll->addWidget(GDataFactory::get_main_window());
    this->setLayout(vAll);
}


void TopWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
}

void TopWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->release_receiver_res();
}
