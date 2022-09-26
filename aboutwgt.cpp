#include "aboutwgt.h"
#include "title/TitleBar.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
AboutWgt::AboutWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()| Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,400);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"¹ØÓÚ");
    pTitleBar->setFixedWidth(800);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    QHBoxLayout* hTop = new QHBoxLayout();
    QLabel* lbIcon = new QLabel();
    lbIcon->setPixmap(QPixmap(":/icon/about.png"));
    QLabel* lbText = new QLabel();
//    lbText->setFont(QFont("Times New Roman",5,QFont::Normal,true));
    lbText->setStyleSheet("font-family:·ÂËÎ;font-size:20px;");
    lbText->setText("Vision Leading software 1.0\r\n\r\n\
Based on Qt 5.12.4 (MSVC 2017, 64 bit) by Qt Creator 4.9.1\r\n\
\r\n\
Built on December 1 2021 00:00:00\r\n\
\r\n\
Copyright 2021-2022 The Hisense Company Ltd. All rights reserved.\r\n\
\r\n\
The program is provided AS IS with NO WARRANTY OF ANY KIND,\r\n\
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY \r\n\
AND FITNESS FOR A PARTICULAR PURPOSE.");
    hTop->addWidget(lbIcon);
    hTop->addWidget(lbText);
    hTop->setStretch(0,3);
    hTop->setStretch(1,7);

    QHBoxLayout* hBottom = new QHBoxLayout();
    QPushButton* btnClose = new QPushButton(u8"   ÍË  ³ö   ");
    connect(btnClose,SIGNAL(clicked()),this,SLOT(slot_close_about()));
    hBottom->addStretch(1);
    hBottom->addWidget(btnClose);

    vAll->addLayout(hTop);
    vAll->addLayout(hBottom);
    this->setLayout(vAll);
}

void AboutWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(207,207,214)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void AboutWgt::slot_close_about()
{
    this->close();
}
