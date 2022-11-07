#include "welcomedlg.h"
#include <QPainter>
#include "gdatafactory.h"
#include <QtMath>
#include <QVBoxLayout>
#include <QGroupBox>
#include "title/TitleBar.h"
#include <QPushButton>
#include <QRegExp>
#include <QValidator>

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

//    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
//    setWindowIcon(QIcon(":/icon/24x24.png"));
//    this->resize(300,200);

//    TitleBar *pTitleBar = new TitleBar(this);
//    pTitleBar->setTitle(u8"XRay点料机");
//    pTitleBar->setFixedWidth(300);
//    installEventFilter(pTitleBar);

//    m_pLineAccount = new QLineEdit();
//    m_pLineAccount->setAlignment(Qt::AlignCenter);
//    m_pLineAccount->setText(u8"");
//    QRegExp regx("[A-Z0-9]+$");
//    QValidator *validator = new QRegExpValidator(regx,m_pLineAccount);
//    m_pLineAccount->setValidator(validator);

//    QGroupBox* grpBox = new QGroupBox(u8"请输入员工编号");
//    grpBox->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
//    QHBoxLayout* hBox = new QHBoxLayout();
//    hBox->addWidget(m_pLineAccount);
//    grpBox->setLayout(hBox);

//    QHBoxLayout* hBox1 = new QHBoxLayout();
//    QPushButton* btnOk = new QPushButton(u8"  确 定  ");
//    connect(btnOk,SIGNAL(clicked()),this,SLOT(slot_ok()));
//    QPushButton* btnCancel = new QPushButton(u8"  取 消  ");
//    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slot_cancel()));
//    hBox1->addWidget(btnOk);
//    hBox1->addWidget(btnCancel);

//    QVBoxLayout* vAll = new QVBoxLayout();
//    vAll->addSpacing(pTitleBar->height());
//    vAll->addWidget(grpBox);
//    vAll->addLayout(hBox1);
//    this->setLayout(vAll);

}

void WelcomeDlg::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

void WelcomeDlg::slot_ok()
{
    this->accept();
}

void WelcomeDlg::slot_cancel()
{
    this->reject();
}

void WelcomeDlg::set_text(int eqID, QString info)
{
}

void WelcomeDlg::timerEvent(QTimerEvent *event)
{
}

void WelcomeDlg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
}

