#include "elecmanualwgt.h"
#include "title/TitleBar.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSignalMapper>
#include "QsLog.h"
#include "gdatafactory.h"
#include <QTimerEvent>
#include <QCloseEvent>
#include <QtEndian>
#include <QDataStream>

ElecManualWgt::ElecManualWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()| Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1300,700);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"电气手动控制");
    pTitleBar->setFixedWidth(1300);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hAll = new QHBoxLayout();

    QVBoxLayout* vLeft = new QVBoxLayout();
    QVBoxLayout* vRight = new QVBoxLayout();

    QGroupBox* grpLeft = new QGroupBox(u8"发射源操作");
    grpLeft->setLayout(vLeft);

    grpLeft->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");

    QHBoxLayout* hBoxFrev = new QHBoxLayout();
    btnFrev = new QPushButton(u8"预设置");
    circleFrev = new CircleWidget;
    hBoxFrev->addWidget(btnFrev);
    hBoxFrev->addWidget(circleFrev);

    QHBoxLayout* hBoxWatchDog = new QHBoxLayout();
    btnWatchDog = new QPushButton(u8"看门狗");
    circleWatchDog = new CircleWidget;
    hBoxWatchDog->addWidget(btnWatchDog);
    hBoxWatchDog->addWidget(circleWatchDog);

    QHBoxLayout* hBoxClr = new QHBoxLayout();
    btnClr = new QPushButton(u8"清 除");
    circleClr = new CircleWidget;
    hBoxClr->addWidget(btnClr);
    hBoxClr->addWidget(circleClr);

    QHBoxLayout* hBoxEnable = new QHBoxLayout();
    btnEnable = new QPushButton(u8"使 能");
    circleEnable = new CircleWidget;
    hBoxEnable->addWidget(btnEnable);
    hBoxEnable->addWidget(circleEnable);

    QHBoxLayout* hBoxOn = new QHBoxLayout();
    btnOn = new QPushButton(u8"打 开");
    circleOn = new CircleWidget;
    hBoxOn->addWidget(btnOn);
    hBoxOn->addWidget(circleOn);

    QHBoxLayout* hBoxVp = new QHBoxLayout();
    btnVp = new QPushButton(u8"电 压");
    circleVp = new CircleWidget;
    hBoxVp->addWidget(btnVp);
    hBoxVp->addWidget(circleVp);

    QHBoxLayout* hBoxCp = new QHBoxLayout();
    btnCp = new QPushButton(u8"电 流");
    circleCp = new CircleWidget;
    hBoxCp->addWidget(btnCp);
    hBoxCp->addWidget(circleCp);

    QHBoxLayout* hBoxUnable = new QHBoxLayout();
    btnUnable = new QPushButton(u8"关 闭");
    circleUnable = new CircleWidget;
    hBoxUnable->addWidget(btnUnable);
    hBoxUnable->addWidget(circleUnable);

    QPushButton* btnReset = new QPushButton(u8"重  置");

    vLeft->addLayout(hBoxFrev);
    vLeft->addLayout(hBoxWatchDog);
    vLeft->addLayout(hBoxClr);
    vLeft->addLayout(hBoxEnable);
    vLeft->addLayout(hBoxOn);
    vLeft->addLayout(hBoxVp);
    vLeft->addLayout(hBoxCp);
    vLeft->addLayout(hBoxUnable);
    vLeft->addWidget(btnReset);

    lstCircleWgtComm<<circleFrev<<circleWatchDog<<circleClr<<circleEnable<<circleOn<<circleVp<<circleCp<<circleUnable;

    lstCircleWgt<<circle1<<circle2<<circle3<<circle4<<circle5<<circle6<<
                    circle7<<circle8<<circle9<<circle10<<circle11<<circle12<<
                    circle13<<circle14<<circle15<<circle16<<circle17<<circle18<<
                    circle19<<circle20<<circle21<<circle22<<circle23<<circle24;
    for(int i=0;i<24;i++)
        lstCircleWgt[i] = new CircleWidget();

    QHBoxLayout* hBoxCircle = new QHBoxLayout();
    QHBoxLayout* hBoxCircle1 = new QHBoxLayout();
    QHBoxLayout* hBoxCircle2 = new QHBoxLayout();
    QHBoxLayout* hBoxCircle3 = new QHBoxLayout();
    for (int i = 0; i < 6; ++i) {
        hBoxCircle->addWidget(lstCircleWgt[i]);
        hBoxCircle1->addWidget(lstCircleWgt[i+6]);
        hBoxCircle2->addWidget(lstCircleWgt[i+12]);
        hBoxCircle3->addWidget(lstCircleWgt[i+18]);

        if(i==0)
        {
            hBoxCircle->addWidget(new QLabel(u8"夹爪垂直返回"));
            hBoxCircle1->addWidget(new QLabel(u8"接料气缸返回"));
            hBoxCircle2->addWidget(new QLabel(u8"安全门检测1 "));
            hBoxCircle3->addWidget(new QLabel(u8"机器1X轴原点"));
        }
        else if(i == 1)
        {
            hBoxCircle->addWidget(new QLabel(u8"夹爪垂直落下"));
            hBoxCircle1->addWidget(new QLabel(u8"接料气缸动作"));
            hBoxCircle2->addWidget(new QLabel(u8"安全门检测2 "));
            hBoxCircle3->addWidget(new QLabel(u8"  启动按钮  "));
        }
        else if(i == 2)
        {
            hBoxCircle->addWidget(new QLabel(u8"夹爪水平返回"));
            hBoxCircle1->addWidget(new QLabel(u8"进料气缸关闭"));
            hBoxCircle2->addWidget(new QLabel(u8"  料检测1   "));
            hBoxCircle3->addWidget(new QLabel(u8"  停止按钮  "));
        }
        else if(i == 3)
        {
            hBoxCircle->addWidget(new QLabel(u8"夹爪水平伸出"));
            hBoxCircle1->addWidget(new QLabel(u8"进料气缸打开"));
            hBoxCircle2->addWidget(new QLabel(u8"  料检测2   "));
            hBoxCircle3->addWidget(new QLabel(u8"  复位按钮  "));
        }
        else if(i == 4)
        {
            hBoxCircle->addWidget(new QLabel(u8"  夹爪关闭  "));
            hBoxCircle1->addWidget(new QLabel(u8"出料气缸关闭"));
            hBoxCircle2->addWidget(new QLabel(u8"机器1X轴上限"));
            hBoxCircle3->addWidget(new QLabel(u8"  确认按钮  "));
        }
        else if(i == 5)
        {
            hBoxCircle->addWidget(new QLabel(u8"  夹爪打开  "));
            hBoxCircle1->addWidget(new QLabel(u8"出料气缸打开"));
            hBoxCircle2->addWidget(new QLabel(u8"机器1X轴下限"));
            hBoxCircle3->addWidget(new QLabel(u8"  急停按钮  "));
        }
    }

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QPushButton* btnScanCodeOver = new QPushButton(u8"扫码完成");
    QPushButton* btnNG = new QPushButton(u8"扫码或无模板NG");
    QPushButton* btnTakePicOver = new QPushButton(u8"接收源拍照完成");
    hBox1->addWidget(btnScanCodeOver);
    hBox1->addWidget(btnNG);
    hBox1->addWidget(btnTakePicOver);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QPushButton* btnCountOver = new QPushButton(u8"点料完成");
    QPushButton* btnCountNG = new QPushButton(u8"点料NG");
    QPushButton* btnStatusError = new QPushButton(u8"状态异常");
    hBox2->addWidget(btnCountOver);
    hBox2->addWidget(btnCountNG);
    hBox2->addWidget(btnStatusError);

    QHBoxLayout* hBox3 = new QHBoxLayout();
    QPushButton* btnJawBack = new QPushButton(u8"夹爪上下气缸返回");
    QPushButton* btnJawDown = new QPushButton(u8"夹爪上下气缸下落");
    QPushButton* btnJawHBack = new QPushButton(u8"夹爪水平返回");
    hBox3->addWidget(btnJawBack);
    hBox3->addWidget(btnJawDown);
    hBox3->addWidget(btnJawHBack);

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QPushButton* btnJawHOut = new QPushButton(u8"夹爪水平伸出");
    QPushButton* btnJawOff = new QPushButton(u8"夹爪关闭");
    QPushButton* btnJawOn = new QPushButton(u8"夹爪打开");
    hBox4->addWidget(btnJawHOut);
    hBox4->addWidget(btnJawOff);
    hBox4->addWidget(btnJawOn);

    QHBoxLayout* hBox5 = new QHBoxLayout();
    QPushButton* btnAcceptProductBack = new QPushButton(u8"接料盘返回");
    QPushButton* btnAcceptProductOut = new QPushButton(u8"接料盘伸出");
    QPushButton* btnInProductOff = new QPushButton(u8"进料门关闭");
    hBox5->addWidget(btnAcceptProductBack);
    hBox5->addWidget(btnAcceptProductOut);
    hBox5->addWidget(btnInProductOff);

    QHBoxLayout* hBox6 = new QHBoxLayout();
    QPushButton* btnInProductOn = new QPushButton(u8"进料门打开");
    QPushButton* btnOutProductOff = new QPushButton(u8"出料门关闭");
    QPushButton* btnOutProductOn = new QPushButton(u8"出料门打开");
    hBox6->addWidget(btnInProductOn);
    hBox6->addWidget(btnOutProductOff);
    hBox6->addWidget(btnOutProductOn);

    QVBoxLayout* vBoxDisplay = new QVBoxLayout();
    QVBoxLayout* vBoxOperation = new QVBoxLayout();
    QGroupBox* grpDisplay = new QGroupBox(u8"信号显示");
    grpDisplay->setLayout(vBoxDisplay);
    QGroupBox* grpOperation = new QGroupBox(u8"电气操作");
    grpOperation->setLayout(vBoxOperation);

    grpDisplay->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");
    grpOperation->setStyleSheet("QGroupBox{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);"
                             "background-color:rgba(0,0,0,0);}");

    vBoxDisplay->addLayout(hBoxCircle);
    vBoxDisplay->addLayout(hBoxCircle1);
    vBoxDisplay->addLayout(hBoxCircle2);
    vBoxDisplay->addLayout(hBoxCircle3);
    vBoxOperation->addLayout(hBox1);
    vBoxOperation->addLayout(hBox2);
    vBoxOperation->addLayout(hBox3);
    vBoxOperation->addLayout(hBox4);
    vBoxOperation->addLayout(hBox5);
    vBoxOperation->addLayout(hBox6);

    vRight->addWidget(grpDisplay);
    vRight->addWidget(grpOperation);

    hAll->addWidget(grpLeft);
    hAll->addLayout(vRight);
    hAll->setStretch(0,1);
    hAll->setStretch(1,4);
    vAll->addSpacing(pTitleBar->height());
    vAll->addLayout(hAll);
    this->setLayout(vAll);

    QList<QPushButton*> lstBtn;
    lstBtn<<btnFrev<<btnWatchDog<<btnClr<<btnEnable<<btnOn<<btnVp<<btnCp<<btnUnable<<btnReset<<
         btnScanCodeOver<<btnNG<<btnTakePicOver<<btnCountOver<<btnCountNG<<btnStatusError<<
            btnJawBack<<btnJawDown<<btnJawHBack<<btnJawHOut<<btnJawOff<<btnJawOn<<
            btnAcceptProductBack<<btnAcceptProductOut<<btnInProductOff<<btnInProductOn<<btnOutProductOff<<btnOutProductOn;
    QSignalMapper* mapper = new QSignalMapper();
    for (int i=0;i<27;i++)
    {
        mapper->setMapping(lstBtn[i],i);
        connect(lstBtn[i],SIGNAL(clicked()),mapper,SLOT(map()));
    }
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(slot_btn_click(int)));

    m_pTimerID = 0;
}

void ElecManualWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100)));
    painter.drawRect(this->rect());
}

void ElecManualWgt::slot_btn_click(int btnID)
{
    QByteArray data;
    data.append(static_cast<char>(0x01));
    switch (btnID) {
    case 0:
        QLOG_INFO()<<u8"FREV";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 1:
        QLOG_INFO()<<u8"Watch dog";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 2:
        QLOG_INFO()<<u8"Clr";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 3:
        QLOG_INFO()<<u8"Enable";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 4:
        QLOG_INFO()<<u8"Open";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 5:
        QLOG_INFO()<<u8"Vp";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 6:
        QLOG_INFO()<<u8"Cp";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 7:
        QLOG_INFO()<<u8"Close";
        GDataFactory::get_factory()->read_serial_number_xray(btnID);
        break;
    case 8:
        QLOG_INFO()<<u8"Reset";
        this->btnFrev->setEnabled(true);
        this->btnWatchDog->setEnabled(true);
        this->btnClr->setEnabled(true);
        this->btnEnable->setEnabled(true);
        this->btnOn->setEnabled(true);
        this->btnVp->setEnabled(true);
        this->btnCp->setEnabled(true);
        this->btnUnable->setEnabled(true);
        for(int i=0;i<8;i++)
            lstCircleWgtComm[i]->set_color(0);
        break;
    case 9:
        QLOG_INFO()<<u8"scan code is over";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W201,ADDRESS_W201_00,data.length(),data);
        break;
    case 10:
        QLOG_INFO()<<u8"scan code or no vision template NG";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W201,ADDRESS_W201_01,data.length(),data);
        break;
    case 11:
        QLOG_INFO()<<u8"receiver take photo is over";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_00,data.length(),data);
        break;
    case 12:
        QLOG_INFO()<<u8"count product is over";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_01,data.length(),data);
        break;
    case 13:
        QLOG_INFO()<<u8"count product NG";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_02,data.length(),data);
        break;
    case 14:
        QLOG_INFO()<<u8"status Error";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_03,data.length(),data);
        break;
    case 15:
        QLOG_INFO()<<u8"jaw up-down cylinder back";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_00,data.length(),data);
        break;
    case 16:
        QLOG_INFO()<<u8"jaw up-down cylinder down";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_01,data.length(),data);
        break;
    case 17:
        QLOG_INFO()<<u8"jaw horizontal back";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_02,data.length(),data);
        break;
    case 18:
        QLOG_INFO()<<u8"jaw horizontal out";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_03,data.length(),data);
        break;
    case 19:
        QLOG_INFO()<<u8"jaw off";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_04,data.length(),data);
        break;
    case 20:
        QLOG_INFO()<<u8"jaw on";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_05,data.length(),data);
        break;
    case 21:
        QLOG_INFO()<<u8"material plate back";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_06,data.length(),data);
        break;
    case 22:
        QLOG_INFO()<<u8"material plate out";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_07,data.length(),data);
        break;
    case 23:
        QLOG_INFO()<<u8"material-in door closed";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_08,data.length(),data);
        break;
    case 24:
        QLOG_INFO()<<u8"material-in door open";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_09,data.length(),data);
        break;
    case 25:
        QLOG_INFO()<<u8"material-out door closed";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_0A,data.length(),data);
        break;
    case 26:
        QLOG_INFO()<<u8"material-out door open";
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W60,ADDRESS_W60_0B,data.length(),data);
        break;
    default:
        break;
    }
}

void ElecManualWgt::slot_process_serial_data(QByteArray data)
{
    QString str = QString::fromUtf8(data);
    if(GDataFactory::get_factory()->get_system_status() == Manual_Status)
    {
        if(str.contains("2000"))
        {
            this->btnFrev->setEnabled(false);
            this->lstCircleWgtComm[0]->set_color(1);
        }
        else if(str.contains("WDOG0"))
        {
            this->btnWatchDog->setEnabled(false);
            this->lstCircleWgtComm[1]->set_color(1);
        }
        else if(str.contains("CLR"))
        {
            this->btnClr->setEnabled(false);
            this->lstCircleWgtComm[2]->set_color(1);
        }
        else if(str.contains("VP"))
        {
            this->btnVp->setEnabled(false);
            this->lstCircleWgtComm[5]->set_color(1);
        }
        else if(str.contains("CP"))
        {
            this->btnCp->setEnabled(false);
            this->lstCircleWgtComm[6]->set_color(1);
        }
        else if(str.contains("ENBL0"))
        {
            this->btnUnable->setEnabled(false);
            this->lstCircleWgtComm[7]->set_color(1);
        }
    }
    else if(GDataFactory::get_factory()->get_system_status() == Init_Status)
    {
        if(str.contains("2000"))
        {
            emit signal_process_serial_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(1);
        }
        else if(str.contains("WDOG0"))
        {
            emit signal_process_serial_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(2);
        }
        else if(str.contains("CLR"))
        {
            emit signal_process_serial_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(5);
        }
        else if(str.contains("VP"))
        {
            emit signal_process_serial_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(6);
        }
        else if(str.contains("CP"))
        {
            emit signal_process_serial_info(1,str);
            emit signal_process_serial_info(1,u8"发射源初始化完成!");
            GDataFactory::get_factory()->set_system_status(Manual_Status);
        }
    }
}

void ElecManualWgt::slot_process_udp_data(QByteArray data)
{
    if(data.size() == 4)
    {
        for(int i=0;i<8;i++)
        {
            int sign = (data[0]>>i) & 0x01;
            if(sign)
                lstCircleWgt[i+8]->set_color(1);
            else
                lstCircleWgt[i+8]->set_color(0);
        }

        for(int i=0;i<8;i++)
        {
            int sign = (data[1]>>i) & 0x01;
            if(sign)
                lstCircleWgt[i]->set_color(1);
            else
                lstCircleWgt[i]->set_color(0);
        }

        for(int i=0;i<8;i++)
        {
            int sign = (data[3]>>i) & 0x01;
            if(sign)
                lstCircleWgt[i+16]->set_color(1);
            else
                lstCircleWgt[i+16]->set_color(0);
        }
    }
}

void ElecManualWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        GDataFactory::get_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_W209,0x04,nullptr);
    }
}

void ElecManualWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pTimerID = startTimer(1000);
}

void ElecManualWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    killTimer(m_pTimerID);
    GDataFactory::get_factory()->set_system_status(Run_Status);
}
