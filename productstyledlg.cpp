#include "productstyledlg.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "gdatafactory.h"
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
ProductStyleDlg::ProductStyleDlg()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground,true);
    this->resize(150,50);

    QLabel* labelProductStyle = new QLabel(u8"请选择点料产品类型:");
    labelProductStyle->setStyleSheet("color:rgb(255,255,255);font-family:KaiTi;font-size:25pt;");
    m_pCmbProductStyle = new QComboBox();
    m_pCmbProductStyle->insertItem(0,"0105");
    m_pCmbProductStyle->insertItem(1,"0402");
    m_pCmbProductStyle->insertItem(2,"0603");
    m_pCmbProductStyle->insertItem(3,"0805");

    connect(m_pCmbProductStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));
    m_pCmbProductStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:KaiTi;font-size:25pt;"
                                           "color:rgba(255,255,255,255);background-color:rgba(0,0,0,0);min-width:120px;"
                                            "max-width:120px;min-height:30px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    QPushButton* btnOK = new QPushButton(u8"    确    定    ");
    btnOK->setStyleSheet("QPushButton{font-family:KaiTi;font-size:25pt;color:rgb(255,255,255);min-height:40px;}QPushButton:hover{border:solid rgba(0,0,255,50);background-color:rgba(0,0,255,50);}");
    connect(btnOK,SIGNAL(clicked()),this,SLOT(slot_ok_click()));
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hAll = new QHBoxLayout();
    QHBoxLayout* hAll1 = new QHBoxLayout();
    hAll->addWidget(labelProductStyle);
    hAll->addWidget(m_pCmbProductStyle);
    hAll1->addStretch(1);
    hAll1->addWidget(btnOK);
    hAll1->addStretch(1);
    vAll->addLayout(hAll);
    vAll->addLayout(hAll1);
    this->setLayout(vAll);
}

void ProductStyleDlg::slot_comb_index_changed(int index)
{
    switch (index) {

    default:
        break;
    }
}

void ProductStyleDlg::slot_ok_click()
{
    this->close();
}

void ProductStyleDlg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(0,0,255,50)));

    painter.setPen(QPen(QColor(0,0,255,100)));
    painter.setFont(QFont(u8"Lucida Calligraphy",40,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter," X-RAY");
}
