#include "processinfowgt.h"
#include <QVBoxLayout>
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

    QHBoxLayout* hTop = new QHBoxLayout();
    QVBoxLayout* vTop = new QVBoxLayout();
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
    QGroupBox* grpLog = new QGroupBox(u8"过程日志");
    QVBoxLayout* vLog = new QVBoxLayout();
    vLog->addWidget(m_pLogEdit);
    grpLog->setLayout(vLog);
    vAll->addWidget(grpLog);
    vAll->setStretch(0,1);
    vAll->setStretch(1,1);
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
