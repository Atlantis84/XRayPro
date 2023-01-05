#include "logindialog.h"
#include "title/TitleBar.h"
#include <QRegExp>
#include <QValidator>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include "imessagebox.h"
#include "gdatafactory.h"
LoginDialog::LoginDialog()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->resize(300,200);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"XRay点料机");
    pTitleBar->setFixedWidth(300);
    installEventFilter(pTitleBar);

    m_pLineAccount = new QLineEdit();
    m_pLineAccount->setAlignment(Qt::AlignCenter);
    m_pLineAccount->setText(u8"");
    QRegExp regx("[A-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,m_pLineAccount);
    m_pLineAccount->setValidator(validator);

    QGroupBox* grpBox = new QGroupBox(u8"请输入员工编号");
    grpBox->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addWidget(m_pLineAccount);
    grpBox->setLayout(hBox);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QPushButton* btnOk = new QPushButton(u8"  确 定  ");
    connect(btnOk,SIGNAL(clicked()),this,SLOT(slot_ok()));
    QPushButton* btnCancel = new QPushButton(u8"  取 消  ");
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slot_cancel()));
    hBox1->addWidget(btnOk);
    hBox1->addWidget(btnCancel);

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpBox);
    vAll->addLayout(hBox1);
    this->setLayout(vAll);
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
}

void LoginDialog::slot_ok()
{
    if(m_pLineAccount->text().trimmed() == "")
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"请输入用户账号!");
        return;
    }

    GDataFactory::get_factory()->set_user_account(this->m_pLineAccount->text().trimmed());
    this->close();
}

void LoginDialog::slot_cancel()
{
    this->close();
}
