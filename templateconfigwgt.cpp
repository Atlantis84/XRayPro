#include "templateconfigwgt.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"
#include "TemplateBasedMethod.h"
#include <QTime>
#include <QShowEvent>
#include "QsLog.h"
TemplateConfigWgt::TemplateConfigWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vTemplate = new QVBoxLayout();
    QVBoxLayout* vOperate = new QVBoxLayout();

    QGroupBox* grpBoxTemplate = new QGroupBox(u8"模板");
    QGroupBox* grpBoxOperate = new QGroupBox(u8"操作");
    grpBoxTemplate->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");
    grpBoxOperate->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");

    vTemplate->addWidget(GDataFactory::get_pixmap_wgt());
    grpBoxTemplate->setLayout(vTemplate);

    QHBoxLayout* hBox_7 = new QHBoxLayout();
    QLabel* labelProStyle = new QLabel(u8"产品类型:");
    labelProStyle->setStyleSheet("background-color:rgba(0,0,0,0)");
    cmb_product_style = new QComboBox();
    cmb_product_style->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    hBox_7->addWidget(labelProStyle);
    hBox_7->addWidget(cmb_product_style);

    QHBoxLayout* hBox_8 = new QHBoxLayout();
    QLabel* labelSenderCurrent = new QLabel(u8"发射源电流:");
    labelSenderCurrent->setStyleSheet("background-color:rgba(0,0,0,0);");
    cmb_sender_current = new QComboBox();
    for(int i=0;i<GDataFactory::get_factory()->get_current_list().length();i++)
        cmb_sender_current->insertItem(i,GDataFactory::get_factory()->get_current_list()[i]);
    cmb_sender_current->setCurrentIndex(0);
    cmb_sender_current->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    hBox_8->addWidget(labelSenderCurrent);
    hBox_8->addWidget(cmb_sender_current);

    QHBoxLayout* hBox_9 = new QHBoxLayout();
    QLabel* labelSenderVoltage = new QLabel(u8"发射源电压:");
    labelSenderVoltage->setStyleSheet("background-color:rgba(0,0,0,0);");
    cmb_sender_voltage = new QComboBox();
    for(int i=0;i<GDataFactory::get_factory()->get_voltage_list().length();i++)
        cmb_sender_voltage->insertItem(i,GDataFactory::get_factory()->get_voltage_list()[i]);
    cmb_sender_voltage->setCurrentIndex(0);
    cmb_sender_voltage->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:100px;}"
                                      "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    hBox_9->addWidget(labelSenderVoltage);
    hBox_9->addWidget(cmb_sender_voltage);

    QHBoxLayout* hBox_10 = new QHBoxLayout();
    QPushButton* btn_config_sender = new QPushButton(u8"设 置 发 射 源");
    btn_config_sender->setStyleSheet("background-color:rgba(0,0,0,0);");
    connect(btn_config_sender,SIGNAL(clicked()),this,SLOT(slot_set_sender_para()));
    hBox_10->addWidget(btn_config_sender);

    QHBoxLayout* hBox_4 = new QHBoxLayout();
    QHBoxLayout* hBox_5 = new QHBoxLayout();
    QHBoxLayout* hBox_6 = new QHBoxLayout();
    QLabel* currentAmount = new QLabel(u8"点料结果:");
    currentAmount->setStyleSheet("background-color:rgba(0,0,0,0)");
    le_product_amount = new QLineEdit();
    le_product_amount->setStyleSheet("background-color:rgba(0,0,0,0);min-height:20px;");
    hBox_6->addWidget(currentAmount);
    hBox_6->addWidget(le_product_amount);

    QPushButton* btn_capture_pic = new QPushButton(u8"接 收 源 采 图");
    QPushButton* btn_generate_template = new QPushButton(u8"生 成 模 板");
    QPushButton* btn_count = new QPushButton(u8"点 料 测 试");
    btn_capture_pic->setStyleSheet("background-color:rgba(0,0,0,0);");
    connect(btn_capture_pic,SIGNAL(clicked()),this,SLOT(slot_receiver_take_image()));
    connect(btn_generate_template,SIGNAL(clicked()),this,SLOT(slot_generate_template()));
    connect(btn_count,SIGNAL(clicked()),this,SLOT(slot_count_test()));
    btn_generate_template->setStyleSheet("background-color:rgba(0,0,0,0);");
    btn_count->setStyleSheet("background-color:rgba(0,0,0,0);");
    hBox_4->addWidget(btn_capture_pic);
    hBox_5->addWidget(btn_count);

    QHBoxLayout* hBox_11 = new QHBoxLayout();
    QLabel* labelStep = new QLabel(u8"点料步长:");
    labelStep->setStyleSheet("background-color:rgba(0,0,0,0);");
    cmb_vision_step = new QComboBox();
    cmb_vision_step->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);}"
                                      "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    float aValue = 0.0;
    for(int i=0;i<10;i++)
    {
        aValue += 0.1;
        cmb_vision_step->insertItem(i,QString("%1").arg(aValue));
    }
    cmb_vision_step->setCurrentIndex(0);

    QLabel* labelThreshold = new QLabel(u8"点料门限:");
    labelThreshold->setStyleSheet("background-color:rgba(0,0,0,0);");
    cmb_vision_threshold = new QComboBox();
    cmb_vision_threshold->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);}"
                                      "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    aValue = 0.49;
    for(int i=0;i<40;i++)
    {
        aValue += 0.01;
        cmb_vision_threshold->insertItem(i,QString("%1").arg(aValue));
    }
    cmb_vision_threshold->setCurrentIndex(0);
    hBox_11->addWidget(labelStep);
    hBox_11->addWidget(cmb_vision_step);
    hBox_11->addWidget(labelThreshold);
    hBox_11->addWidget(cmb_vision_threshold);

    vOperate->addLayout(hBox_7);
    vOperate->addLayout(hBox_9);
    vOperate->addLayout(hBox_8);
    vOperate->addLayout(hBox_10);
    vOperate->addLayout(hBox_4);
    vOperate->addLayout(hBox_11);
    vOperate->addLayout(hBox_5);
    vOperate->addLayout(hBox_6);
    grpBoxOperate->setLayout(vOperate);

//    vAll->addWidget(grpBoxSet);
    vAll->addWidget(grpBoxTemplate);
    vAll->addWidget(grpBoxOperate);
//    vAll->setStretch(0,1);
    vAll->setStretch(0,2);
    vAll->setStretch(1,3);
    this->setLayout(vAll);

    m_pCountThread = new CountThread();
    connect(m_pCountThread,SIGNAL(signal_count_over(int)),this,SLOT(slot_count_over(int)));
}

TemplateConfigWgt::~TemplateConfigWgt()
{
}

void TemplateConfigWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());
}

void TemplateConfigWgt::slot_receiver_take_image()
{
    GDataFactory::get_zoom_obj()->set_product_style(this->cmb_product_style->currentText());
    GDataFactory::get_factory()->set_sender_power(this->cmb_product_style->currentText());
    emit signal_notify_take_picture();
}

void TemplateConfigWgt::slot_generate_template()
{
    //generate template
}

void TemplateConfigWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    cmb_product_style->clear();
    QMap<QString,QString> tmpProductInfo;
    tmpProductInfo = GDataFactory::get_factory()->get_product_info();
    QMap<QString,QString>::Iterator itr = tmpProductInfo.begin();
    QList<QString> lstProductStyle;
    while(itr != tmpProductInfo.end())
    {
        if(lstProductStyle.contains(itr.value()))
        {
            itr++;
            continue;
        }
        lstProductStyle.push_back(itr.value());
        itr++;
    }

    for(int i=0;i<lstProductStyle.length();i++)
        cmb_product_style->insertItem(i,lstProductStyle[i]);

    cmb_product_style->setCurrentIndex(0);
}

void TemplateConfigWgt::update_power_para()
{
    //set sender para
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
            arg(constProductStyleMapTable).arg(this->cmb_product_style->currentText());
    QSqlQuery queryResult;
    QString strError;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_INFO()<<"query database SUCCESS!";
        strSql = QString("update public.%1 set \"Vision_Step\"='%2',\"Vision_Threshold\"='%3' where \"Product_Style_Code\"='%4'").
                arg(constProductStylePowerTable).arg(this->cmb_vision_step->currentText()).
                arg(this->cmb_vision_threshold->currentText()).arg(this->cmb_product_style->currentText());
        if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
        {
            QLOG_INFO()<<"update DB SUCCESS!";
        }
        else
        {
            QLOG_WARN()<<"update DB FAILED!";
        }
    }
    else{
        QLOG_WARN()<<"query database FAILED!";
    }
}

void TemplateConfigWgt::get_step_and_threshold(float &step, float &threshold)
{
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
            arg(constProductStylePowerTable).arg(this->cmb_product_style->currentText());
    QSqlQuery queryResult;
    QString strError;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        if(queryResult.size() == 0)
        {
            step = this->cmb_vision_step->currentText().toFloat();
            threshold = this->cmb_vision_threshold->currentText().toFloat();
            strSql = QString("insert into public.%1 values('%2','%3','%4','%5','%6')").
                    arg(constProductStylePowerTable).
                    arg(this->cmb_product_style->currentText()).
                    arg(this->cmb_sender_voltage->currentText()).
                    arg(this->cmb_sender_current->currentText()).
                    arg(this->cmb_vision_step->currentText()).
                    arg(this->cmb_vision_threshold->currentText());
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
                ;
            else
                QLOG_WARN()<<"insert into DB FAILED!";
        }
        else {
            while(queryResult.next())
            {
                step = queryResult.value(3).toFloat();
                threshold = queryResult.value(4).toFloat();
            }
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

static cv::Mat currentFullImage;
static cv::Mat tempMat;
static QString configProductStyle;
static float m_pVisionStep;
static float m_pVisionThreshold;

void TemplateConfigWgt::slot_count_test()
{
    update_power_para();

    configProductStyle = this->cmb_product_style->currentText();
    this->le_product_amount->setText("");
    QString tmpProductStyle = this->cmb_product_style->currentText();
    tmpProductStyle.prepend("f:/template/");
    tmpProductStyle.append(".png");
    tempMat = cv::imread(tmpProductStyle.toStdString());

    QString tmp = this->cmb_product_style->currentText();
    tmp.prepend("f:/Initial/");
    tmp.append(".jpg");
    currentFullImage = cv::imread(tmp.toStdString());

    if(currentFullImage.data && tempMat.data)
    {
        get_step_and_threshold(m_pVisionStep,m_pVisionThreshold);
        GDataFactory::get_wait_count_wgt()->show();
        m_pCountThread->start();
    }
}

void TemplateConfigWgt::slot_set_sender_para()
{
    GDataFactory::get_factory()->set_current_product_style(this->cmb_product_style->currentText());
    //set sender para
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
            arg(constProductStylePowerTable).arg(this->cmb_product_style->currentText());
    QSqlQuery queryResult;
    QString strError;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_INFO()<<"query database SUCCESS!";
        if(queryResult.size() == 0)//insert new row
        {
            strSql = QString("insert into %1 values('%2','%3','%4')").
                    arg(constProductStylePowerTable).arg(this->cmb_product_style->currentText()).
                    arg(this->cmb_sender_voltage->currentText()).arg(this->cmb_sender_current->currentText());
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
            {
                QLOG_INFO()<<"insert into DB SUCCESS!";
            }
            else
            {
                QLOG_WARN()<<"insert into DB FAILED!";
            }
        }
        else//update current row
        {
            strSql = QString("update public.%1 set \"Sender_Voltage\"='%2',\"Sender_Current\"='%3' where \"Product_Style_Code\"='%4'").
                    arg(constProductStylePowerTable).arg(this->cmb_sender_voltage->currentText()).
                    arg(this->cmb_sender_current->currentText()).arg(this->cmb_product_style->currentText());
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
            {
                QLOG_INFO()<<"update DB SUCCESS!";
            }
            else
            {
                QLOG_WARN()<<"update DB FAILED!";
            }
        }
    }
    else{
        QLOG_WARN()<<"query database FAILED!";
    }

    GDataFactory::get_factory()->set_sender_current(this->cmb_sender_current->currentText());
    GDataFactory::get_factory()->set_sender_voltage(this->cmb_sender_voltage->currentText());
    GDataFactory::get_factory()->read_serial_number_xray(5);
    QThread::msleep(500);
    GDataFactory::get_factory()->read_serial_number_xray(6);
}

void TemplateConfigWgt::slot_count_over(int amount)
{
//    m_pCountThread->quit();
    m_pCountThread->terminate();
    this->le_product_amount->setText(QString("%1").arg(amount));
    GDataFactory::get_wait_count_wgt()->hide();
}

CountThread::CountThread(QWidget *parent)
{

}

void CountThread::run()
{
    while(1)
    {
        QTime tm;
        tm.start();
        TemplateBasedMethod t;
        if(configProductStyle == "0402" ||
                configProductStyle == "0603"||
                configProductStyle == "0805" ||
                configProductStyle == "1206")
        {
            GDataFactory::get_factory()->pre1(currentFullImage);
            GDataFactory::get_factory()->pre2(currentFullImage);
            GDataFactory::get_factory()->pre3(currentFullImage);
        }
        else
        {
            GDataFactory::get_factory()->pre1(currentFullImage);
            GDataFactory::get_factory()->pre_big(currentFullImage);
        }
        t.setImage(currentFullImage);
        t.setTemplateImage(tempMat);
        t.setMaxMatch(20000);
        t.setAngleStep(m_pVisionStep);
        t.setThreshold(m_pVisionThreshold);
        t.CreateNCCTemplate();
        t.TemplateMatchWithNCC();

        UnitInfo info = t.getCheckResult();
        QLOG_WARN()<<"the count time is:"<<tm.elapsed()<<" ms";
        emit signal_count_over(info.units_num);
        break;
    }
}
