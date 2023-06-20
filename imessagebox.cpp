#include "imessagebox.h"
#include ".\title\TitleBar.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QTextEdit>

IMessageBox::IMessageBox(int infoLevel)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(400,220);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setFixedWidth(400);
    pTitleBar->setTitle("IMessageBox");

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QHBoxLayout* hBox2 = new QHBoxLayout();

    QLabel* labelIcon = new QLabel();
    labelIcon->setScaledContents(true);
    labelIcon->setMinimumSize(80,80);
    labelIcon->setMaximumSize(80,80);
    labelIcon->setStyleSheet("background-color:rgba(0,0,0,0)");
    labelInfo = new QLabel();
    labelInfo->setMinimumSize(50,50);
    labelInfo->setWordWrap(true);
    labelInfo->setStyleSheet("background-color:rgba(0,0,0,0)");

    QPushButton* btnOk = new QPushButton(u8"确 定");
    btnOk->setStyleSheet("color:rgba(0,0,0,255);background-color:rgba(0,0,0,0)");
    connect(btnOk,SIGNAL(clicked()),this,SLOT(slot_OK()));
    hBox2->addStretch(9);
    hBox2->addWidget(btnOk);
    hBox2->addStretch(1);

    switch (infoLevel)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        labelIcon->setPixmap(QPixmap(":/icon/start.png"));
        hBox1->addWidget(labelIcon);
        hBox1->addWidget(labelInfo);
        hBox1->setStretch(0,3);
        hBox1->setStretch(1,7);
        break;
    case 3:
        labelIcon->setPixmap(QPixmap(":/icon/error.png"));
        hBox1->addWidget(labelIcon);
        hBox1->addWidget(labelInfo);
        hBox1->setStretch(0,3);
        hBox1->setStretch(1,7);
        break;
    case 4:
        break;
    default:
        break;
    }

    vAll->addSpacing(pTitleBar->height());
    vAll->addLayout(hBox1);
    vAll->addLayout(hBox2);
    vAll->setStretch(0,4);
    vAll->setStretch(1,6);
    this->setLayout(vAll);
    installEventFilter(pTitleBar);
}

void IMessageBox::warning(QString strInfo)
{

    labelInfo->setText(strInfo);
//    this->show();
    this->exec();
}

void IMessageBox::error(QString strInfo)
{
    labelInfo->setText(strInfo);
    this->exec();
}

void IMessageBox::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void IMessageBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));

    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void IMessageBox::slot_OK()
{
    this->accept();
}

