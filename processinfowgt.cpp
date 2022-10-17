#include "processinfowgt.h"
#include <QVBoxLayout>
#include "gdatafactory.h"
#include "QsLog.h"
ProcessInfoWgt::ProcessInfoWgt(QWidget *parent) : QWidget(parent)
{
    m_pLeftWgt = new LeftWidget();

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->setMargin(0);
    m_pLogEdit = new QTextEdit();
    m_pLogEdit->setReadOnly(true);
    m_pLogEdit->setStyleSheet("color:rgb(255,0,0);background-color:rgba(0,0,0,150);font: 12pt 'LiSu'");
    m_pProductAmount = new QLineEdit();
    m_pProductAmount->setReadOnly(true);
    m_pProductAmount->setAlignment(Qt::AlignCenter);
    m_pProductAmount->setStyleSheet("color:rgb(1,158,161);font-weight:bold;font-family:Microsoft YaHei;font-size:30px;background-color:rgba(0,0,0,0);");
    m_pCountTime = new QLineEdit();
    m_pCountTime->setReadOnly(true);
    m_pCountTime->setAlignment(Qt::AlignCenter);
    m_pCountTime->setStyleSheet("color:rgb(1,158,161);font-weight:bold;font-family:Microsoft YaHei;font-size:30px;background-color:rgba(0,0,0,0);");
    m_pProductStyle = new QLineEdit();
    m_pProductStyle->setReadOnly(true);
    m_pProductStyle->setAlignment(Qt::AlignCenter);
    m_pProductStyle->setStyleSheet("color:rgb(1,158,161);font-weight:bold;font-family:Microsoft YaHei;font-size:30px;background-color:rgba(0,0,0,0);");
    m_pCountStep = new QLineEdit();
    m_pCountStep->setReadOnly(true);
    m_pCountStep->setAlignment(Qt::AlignCenter);
    m_pCountStep->setStyleSheet("color:rgb(1,158,161);font-weight:bold;font-family:Microsoft YaHei;font-size:30px;background-color:rgba(0,0,0,0);");


    QLabel* labelProductAmount = new QLabel(u8"当前料数");
    labelProductAmount->setAlignment(Qt::AlignCenter);
    labelProductAmount->setStyleSheet("font-weight:bold;color:rgb(1,158,161);background-color:rgba(0,0,0,0);font-family:Microsoft YaHei;font-size:20pt;");
    QLabel* labelCountTime = new QLabel(u8"当前点料时间");
    labelCountTime->setAlignment(Qt::AlignCenter);
    labelCountTime->setStyleSheet("font-weight:bold;color:rgb(1,158,161);background-color:rgba(0,0,0,0);font-family:Microsoft YaHei;font-size:20pt;");
    QLabel* labelProductStyle = new QLabel(u8"当前产品类型");
    labelProductStyle->setAlignment(Qt::AlignCenter);
    labelProductStyle->setStyleSheet("font-weight:bold;color:rgb(1,158,161);background-color:rgba(0,0,0,0);font-family:Microsoft YaHei;font-size:20pt;");
    QLabel* labelCountStep = new QLabel(u8"当前工作部节拍");
    labelCountStep->setAlignment(Qt::AlignCenter);
    labelCountStep->setStyleSheet("font-weight:bold;color:rgb(1,158,161);background-color:rgba(0,0,0,0);font-family:Microsoft YaHei;font-size:20pt;");

    QHBoxLayout* hBoxThreshold = new QHBoxLayout();
    QLabel* lProductStyle = new QLabel(u8"产品类型:");
    lProductStyle->setStyleSheet("background-color:rgba(0,0,0,0);");
    m_pCmbProductStyle = new QComboBox();
    for(int i=0;i<GDataFactory::get_factory()->get_product_style_list().size();i++)
        m_pCmbProductStyle->insertItem(i,GDataFactory::get_factory()->get_product_style_list()[i]);
    m_pCmbProductStyle->setCurrentIndex(-1);
    m_pCmbProductStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:120px;}"
                                      "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    QLabel* lCurrentThreshold = new QLabel(u8"当前门限:");
    lCurrentThreshold->setStyleSheet("background-color:rgba(0,0,0,0);");
    m_pCurrentThreshold = new QLineEdit();
    m_pCurrentThreshold->setReadOnly(true);
    m_pCurrentThreshold->setAlignment(Qt::AlignCenter);
    m_pCurrentThreshold->setStyleSheet("min-width:100px;font-weight:bold;font-family:Microsoft YaHei;font-size:20px;background-color:rgba(0,0,0,0);");

    QLabel* lThreshold = new QLabel(u8"设置门限:");
    lThreshold->setStyleSheet("background-color:rgba(0,0,0,0);");
    m_pCmbThreshold = new QComboBox();
    float aValue = 0.49;
    for(int i=0;i<40;i++)
    {
        aValue += 0.01;
        m_pCmbThreshold->insertItem(i,QString("%1").arg(aValue));
    }
    m_pCmbThreshold->setCurrentIndex(-1);
    m_pCmbThreshold->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                          "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                          "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    btnUpdate = new QPushButton(u8" 修改门限值 ");
    connect(btnUpdate,SIGNAL(clicked()),this,SLOT(slot_update_threshold()));
    btnUpdate->setStyleSheet("background-color:rgba(0,0,0,0);min-width:100px;");
    hBoxThreshold->addStretch();
    hBoxThreshold->addWidget(lProductStyle);
    hBoxThreshold->addWidget(m_pCmbProductStyle);
    hBoxThreshold->addWidget(lCurrentThreshold);
    hBoxThreshold->addWidget(m_pCurrentThreshold);
    hBoxThreshold->addWidget(lThreshold);
    hBoxThreshold->addWidget(m_pCmbThreshold);
    hBoxThreshold->addWidget(btnUpdate);
    hBoxThreshold->addStretch();

    QHBoxLayout* hTop = new QHBoxLayout();
    QVBoxLayout* vTop = new QVBoxLayout();
//    vTop->addLayout(hBoxThreshold);
    vTop->addWidget(labelProductStyle);
    vTop->addWidget(m_pProductStyle);
    vTop->addWidget(labelProductAmount);
    vTop->addWidget(m_pProductAmount);
    vTop->addWidget(labelCountTime);
    vTop->addWidget(m_pCountTime);
    vTop->addWidget(labelCountStep);
    vTop->addWidget(m_pCountStep);

    hTop->addLayout(vTop);
    hTop->addWidget(m_pLeftWgt);
    hTop->setStretch(0,1);
    hTop->setStretch(1,1);
    vAll->addLayout(hTop);

    QGroupBox* grpPara = new QGroupBox(u8"产品点料门限设置");
    grpPara->setLayout(hBoxThreshold);
    vAll->addWidget(grpPara);
    QGroupBox* grpLog = new QGroupBox(u8"过程日志");
    QVBoxLayout* vLog = new QVBoxLayout();
    vLog->addWidget(m_pLogEdit);
    grpLog->setLayout(vLog);
    vAll->addWidget(grpLog);
    vAll->setStretch(0,3);
    vAll->setStretch(1,1);
    vAll->setStretch(2,3);
    this->setLayout(vAll);
}

void ProcessInfoWgt::slot_rev_logs(const QString log)
{
    this->m_pLogEdit->append(log);
}

void ProcessInfoWgt::slot_rev_count_result(int countR, double aTime)
{
    m_pProductAmount->setText(QString::number(countR));
    m_pCountTime->setText(QString("%1").arg(aTime));
    this->m_pLeftWgt->set_count_total();
}

void ProcessInfoWgt::slot_rev_bar_code(QString pStyle)
{
    m_pProductStyle->setText(pStyle);
}

void ProcessInfoWgt::slot_rev_count_step(QString step)
{
    m_pCountStep->setText(step);
}

void ProcessInfoWgt::slot_update_threshold()
{
    QString strSql = QString("update public.%1 set \"Vision_Threshold\"='%2' where \"Product_Style_Code\"='%3'").
            arg(constProductStylePowerTable).
            arg(this->m_pCmbThreshold->currentText()).
            arg(this->m_pCmbProductStyle->currentText());
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        QLOG_INFO()<<"update DB SUCCESS!";
    }
    else
    {
        QLOG_WARN()<<"update DB FAILED!";
    }
}

void ProcessInfoWgt::slot_rev_button_control_sign(bool sign)
{
    this->btnUpdate->setEnabled(sign);
}

void ProcessInfoWgt::slot_rev_current_threshold(QString tr)
{
//    QLOG_WARN()<<"current threshold is:"<<tr;
    m_pCurrentThreshold->setText(tr);
}
