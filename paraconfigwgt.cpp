#include "paraconfigwgt.h"
#include "title/TitleBar.h"
#include <QPainter>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "gdatafactory.h"
#include <QDebug>
#include <QCheckBox>
#include <QDir>
#include <QApplication>
#include "QsLog.h"
ParaConfigWgt::ParaConfigWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,400);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"参数配置");
    pTitleBar->setFixedWidth(800);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QLabel* labelPLCAddr = new QLabel(u8"通信地址:");
    lineEditPLCAddr = new QLineEdit();
    QLabel* labelPLCPort = new QLabel(u8"通信端口:");
    lineEditPLCPort = new QLineEdit();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(labelPLCAddr);
    hBox1->addWidget(lineEditPLCAddr);
    hBox1->addWidget(labelPLCPort);
    hBox1->addWidget(lineEditPLCPort);


    cmbProductStyle = new QComboBox();
    cmbProductStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                                               "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                                               "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    cmbVotage = new QComboBox();
    cmbVotage->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    cmbCurrent = new QComboBox();
    cmbCurrent->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    QLabel* labelVisualAddr = new QLabel(u8"接收源电压:");
    QLabel* vUnit = new QLabel(u8"HV");
    QLabel* cUnit = new QLabel(u8"MA");
    QLabel* labelVisualPort = new QLabel(u8"接收源电压:");
    QHBoxLayout* hBox2 = new QHBoxLayout();

    QLabel* labelProductStyle = new QLabel(u8"产品类型:");
    hBox2->addWidget(labelProductStyle);
    hBox2->addWidget(cmbProductStyle);

    for(int i=0;i<GDataFactory::get_factory()->get_voltage_list().length();i++)
        cmbVotage->insertItem(i,GDataFactory::get_factory()->get_voltage_list()[i]);
    cmbVotage->setCurrentIndex(0);
    hBox2->addWidget(labelVisualAddr);
    hBox2->addWidget(cmbVotage);
    hBox2->addWidget(vUnit);
    hBox2->addWidget(labelVisualPort);
    for(int i=0;i<GDataFactory::get_factory()->get_current_list().length();i++)
        cmbCurrent->insertItem(i,GDataFactory::get_factory()->get_current_list()[i]);
    cmbCurrent->setCurrentIndex(0);
    hBox2->addWidget(cmbCurrent);
    hBox2->addWidget(cUnit);

    QLabel* labelCCAddr = new QLabel(u8"通信地址:");
    lineEditCCAddr = new QLineEdit();
    QLabel* labelCCPort = new QLabel(u8"通信端口:");
    lineEditCCPort = new QLineEdit();
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addWidget(labelCCAddr);
    hBox3->addWidget(lineEditCCAddr);
    hBox3->addWidget(labelCCPort);
    hBox3->addWidget(lineEditCCPort);

    QPushButton* btnOK = new QPushButton(u8" 确  定 ");
    connect(btnOK,SIGNAL(clicked()), this, SLOT(slot_SetParaValue()));
    QPushButton* btnCancel = new QPushButton(u8" 取  消 ");
    connect(btnCancel,SIGNAL(clicked()), this, SLOT(slot_CloseWgt()));
    QHBoxLayout* hBox4 = new QHBoxLayout();
    hBox4->addStretch(1);
    hBox4->addWidget(btnOK);
    hBox4->addStretch(1);
    hBox4->addWidget(btnCancel);
    hBox4->addStretch(1);

    QGroupBox* grpPLC = new QGroupBox(u8"PLC配置");
    grpPLC->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpPLC->setLayout(hBox1);
//    QGroupBox* grpVisual = new QGroupBox(u8"发射源功率配置");
//    grpVisual->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
//    grpVisual->setLayout(hBox2);
    QGroupBox* grpCC = new QGroupBox(u8"发射源通信配置");
    grpCC->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpCC->setLayout(hBox3);

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpPLC);
    vAll->addWidget(grpCC);
//    vAll->addWidget(grpVisual);
    vAll->addLayout(hBox4);
    this->setLayout(vAll);
}

void ParaConfigWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void ParaConfigWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    QString binDir=QApplication::applicationDirPath();
    QString fileName("/config.json");
    binDir.append(fileName);

    GDataFactory::get_factory()->load_json_config(QByteArray(binDir.toLatin1()).data());
    this->set_ui_para();
}

void ParaConfigWgt::set_ui_para()
{
    this->lineEditPLCAddr->setText(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    this->lineEditPLCPort->setText(GDataFactory::get_factory()->get_config_para("PLC_PORT"));
//    this->lineEditVisualAddr->setText(GDataFactory::get_factory()->get_config_para("VISUAL_IP"));
//    this->lineEditVisualPort->setText(GDataFactory::get_factory()->get_config_para("VISUAL_PORT"));
    this->lineEditCCAddr->setText(GDataFactory::get_factory()->get_config_para("COM_PORT"));
    this->lineEditCCPort->setText(GDataFactory::get_factory()->get_config_para("COM_BARD_RATE"));
}

void ParaConfigWgt::slot_SetParaValue()
{
    GDataFactory::get_factory()->set_config_para("PLC_IP",this->lineEditPLCAddr->text());
    GDataFactory::get_factory()->set_config_para("PLC_PORT",this->lineEditPLCPort->text());
    GDataFactory::get_factory()->set_config_para("COM_PORT",this->lineEditCCAddr->text());
    GDataFactory::get_factory()->set_config_para("COM_BARD_RATE",this->lineEditCCPort->text());
    GDataFactory::get_factory()->save_config_file();
    this->close();
}

void ParaConfigWgt::slot_CloseWgt()
{
    this->close();
}
