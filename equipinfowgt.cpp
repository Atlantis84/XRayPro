#include "equipinfowgt.h"
#include <QPaintEvent>
#include <QPainter>
#include <QHBoxLayout>
EquipInfoWgt::EquipInfoWgt(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hAll = new QHBoxLayout();
    m_pLabelInfo = new QLabel(this);
    m_pLabelInfo->setStyleSheet("color:rgb(255,0,0);min-width:800px;");
    hAll->addWidget(m_pLabelInfo);
}

void EquipInfoWgt::set_text(int eqID, QString info)
{
    this->m_pCurrentEquipID = eqID;
    this->m_pCurrentInfo = info;
    this->m_pLabelInfo->setText(info);
//    this->repaint();
}

void EquipInfoWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(QColor(0,0,255)));
    painter.setFont(QFont(u8"隶书",30,QFont::Bold,false));

    if(m_pCurrentEquipID == 1)//the serial port
    {
        QString str = u8"发射源初始化";
        str.append(m_pCurrentInfo);
        painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignTop,str);
    }
    else if(m_pCurrentEquipID == 2)
        painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignTop,m_pCurrentInfo);
}

void EquipInfoWgt::slot_process_info(int equipID, QString info)
{
//    this->m_pLabelInfo->setText(info);
    this->m_pCurrentEquipID = equipID;
    this->m_pCurrentInfo = info;
    this->repaint();
}
