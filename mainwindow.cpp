#include "mainwindow.h"
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QLabel>
#include <QToolButton>
#include "gdatafactory.h"
#include <QPainter>
#include <QTimerEvent>
#include "QsLog.h"
#include <QCloseEvent>
#include "split.h"
#include <QFile>
#include <QFileInfo>
#include "imessagebox.h"
#include <QCoreApplication>
#include <QTimer>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->setStyleSheet("QMainWindow{background-color:#000000}");
    createStatusBar();
    createActions();
    this->setCentralWidget(GDataFactory::get_central_wgt());
    m_pTimeOut = 0;
    m_pNormalTakePictureSign = false;
}

MainWindow::~MainWindow()
{

}

void MainWindow::createStatusBar()
{
    QFont font("LiSu", 16, QFont::StyleNormal);
    statusBar()->setFont(font);
    statusBar()->setStyleSheet("QStatusBar{color:rgb(0,0,0);border:1px solid rgb(0,0,0,100);"
                               "background-color:rgb(216,217,222);}");
    statusBar()->showMessage(u8"准备好");
    QLabel *lb1 = new QLabel(u8"Hisense|",this);
    lb1->setStyleSheet("color:rgb(0,0,0);");
    QLabel *lb2 = new QLabel(u8"copyright@2022智动精工---技术装备部",this);
    lb2->setStyleSheet("color:rgb(0,0,0);");
    lb1->setFont(font);
    lb2->setFont(font);
    statusBar()->addPermanentWidget(lb1);
    statusBar()->addPermanentWidget(lb2);
}

void MainWindow::createActions()
{
    QWidget *wtSpace[10];
    for (int i=0;i<10;i++) {
        wtSpace[i] = new QWidget();
        wtSpace[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    }
    menuBar()->setStyleSheet("QMenuBar{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 rgb(235,236,240),stop:0.5 rgb(207,207,214),stop:1 rgb(235,236,240));}");
    QMenu *fileMenu = menuBar()->addMenu(u8"&用户操作");
    QToolBar *fileToolBar = addToolBar(u8"..");
    fileToolBar->setStyleSheet("QToolBar{border:solid rgba(0,0,0,150);background-color:rgb(235,236,240);}\
                               QToolButton{background-color:rgba(22,194,194,0);\
                               color:rgb(0,0,0);}");
    menuBar()->setFont(QFont (u8"KaiTi", 15, QFont::StyleNormal));
    fileMenu->setFont(QFont (u8"KaiTi", 15, QFont::StyleNormal));
    const QIcon userIcon = QIcon::fromTheme("document-new", QIcon(":/icon/enmergency.png"));
    const QIcon startIcon = QIcon::fromTheme("",QIcon(":/icon/template.png"));
    const QIcon runIcon = QIcon::fromTheme("",QIcon(":/icon/start.png"));
    const QIcon productManagementIcon = QIcon::fromTheme("",QIcon(":/icon/tooling.png"));

    startRunAct = new QAction(runIcon,u8"&开始运行",this);
    elecManualAct = new QAction(userIcon, u8"&电气手动控制", this);
    visionTemplateAct = new QAction(startIcon, u8"视觉模板制作",this);
    productManagementAct = new QAction(productManagementIcon,u8"盘料产品管理",this);

    if(GDataFactory::get_factory()->get_config_para("AUTHORIZATION") == "1")
    {
        elecManualAct->setEnabled(false);
        visionTemplateAct->setEnabled(false);
    }
    else{
        elecManualAct->setEnabled(true);
        visionTemplateAct->setEnabled(true);
    }

    startRunAct->setStatusTip(u8"开始运行");
    elecManualAct->setStatusTip(u8"电气手动控制");
    visionTemplateAct->setStatusTip(u8"视觉模板制作");
    productManagementAct->setStatusTip(u8"盘料产品管理");

    connect(startRunAct,SIGNAL(triggered()),this,SLOT(slot_start_run()));
    connect(elecManualAct,SIGNAL(triggered()), this, SLOT(slot_elec_manual_control()));
    connect(visionTemplateAct,SIGNAL(triggered()), this, SLOT(slot_create_vision_template()));
    connect(productManagementAct,SIGNAL(triggered()), this, SLOT(slot_product_management()));

    fileMenu->addAction(startRunAct);
    fileMenu->addAction(elecManualAct);
    fileMenu->addAction(visionTemplateAct);
    fileMenu->addAction(productManagementAct);

    fileToolBar->addAction(startRunAct);
    fileToolBar->addAction(elecManualAct);
    fileToolBar->addAction(visionTemplateAct);
    fileToolBar->addAction(productManagementAct);
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolBar->setIconSize(QSize(50,50));

    fileMenu->addSeparator();

    QAction *quitAct = new QAction(u8"&退出",this);
    fileMenu->addAction(quitAct);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(u8"退出应用程序");
    connect(quitAct,SIGNAL(triggered()),this,SLOT(exitApp()));

    QMenu *orderMenu = menuBar()->addMenu(u8"&参数配置");
    orderMenu->setFont(QFont (u8"KaiTi", 15, QFont::StyleNormal));
    const QIcon portSetIcon = QIcon::fromTheme("",QIcon(":/icon/order_edit.ico"));

    portSetAct = new QAction(portSetIcon,u8"设备参数配置",this);
    if(GDataFactory::get_factory()->get_config_para("AUTHORIZATION") == "1")
        portSetAct->setEnabled(false);
    else
        portSetAct->setEnabled(true);

    QLabel* hisenseLogo = new QLabel();
    hisenseLogo->setStyleSheet("QLabel{min-width:350px;min-height:100px;background-color:rgba(0,0,0,0);border:none;}");
    QPixmap *hisensePixmap = new QPixmap(":/icon/2.png");
    hisensePixmap->scaled(hisenseLogo->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    hisenseLogo->setPixmap(*hisensePixmap);

    portSetAct->setStatusTip(u8"设备参数配置");
    connect(portSetAct,SIGNAL(triggered()), this, SLOT(slot_para_set()));
    orderMenu->addAction(portSetAct);
    fileToolBar->addAction(portSetAct);
    fileToolBar->addWidget(wtSpace[3]);
    fileToolBar->addWidget(hisenseLogo);

//    viewMenu = menuBar()->addMenu(u8"&视图");
//    viewMenu->setFont(QFont (u8"KaiTi", 15, QFont::StyleNormal));

    QMenu *helpMenu = menuBar()->addMenu(u8"&帮助");
    QAction *aboutAct = new QAction(u8"&关于",this);
    helpMenu->addAction(aboutAct);
    helpMenu->setFont(QFont (u8"KaiTi", 15, QFont::StyleNormal));
    aboutAct->setStatusTip(u8"显示应用程序关于对话框");
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(about()));
}

void MainWindow::about()
{
    GDataFactory::get_about_wgt()->show();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(GDataFactory::get_factory()->get_system_status() != Run_Status)
        return;
    if(event->timerId() == m_pTimerID)
    {
        m_pTimeOut = 0;
        GDataFactory::get_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D200,0x02,nullptr);
        killTimer(m_pTimerID);
    }
    else if(event->timerId() == m_pTimerTakePictureOverID)
    {
        if(m_pNormalTakePictureSign == true)
        {
            QLOG_WARN()<<u8"--- 点料结束 ---";
            killTimer(m_pTimerTakePictureOverID);
            m_pNormalTakePictureSign = false;

            GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_00,m_pData.length(),m_pData);
            QThread::msleep(200);
            GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_01,m_pData.length(),m_pData);
            m_pTimerID = startTimer(1000);
        }
    }
    else if(event->timerId() == m_pTimerMonitorID)
    {
        return;
        m_pTimeOut++;
        if(m_pTimeOut == 5)
        {
            QLOG_WARN()<<"PLC reply time out! restart query Timer!";
            m_pTimeOut = 0;
            m_pTimerID = startTimer(1000);
        }
    }
}

static QString t;
QString test_result(int i)
{
    if(i!=0)
        ;
    else {
        return "";
    }
}

void MainWindow::slot_start_run()
{
//    GDataFactory::get_factory()->submit_msg_to_mes("12345","5000");
//    return;
    if(this->startRunAct->text().contains(u8"开始运行"))
    {
        GDataFactory::get_factory()->set_system_status(Run_Status);
        this->startRunAct->setText(u8"停止运行");
        m_pTimerID = startTimer(1000);
        m_pTimerMonitorID = startTimer(1000);
        this->elecManualAct->setEnabled(false);
        this->visionTemplateAct->setEnabled(false);
        this->portSetAct->setEnabled(false);
        this->productManagementAct->setEnabled(false);
        emit signal_enable_disable_threshold_button(false);
    }
    else
    {
        GDataFactory::get_factory()->set_system_status(Manual_Status);
        this->startRunAct->setText(u8"开始运行");
        killTimer(m_pTimerID);
        killTimer(m_pTimerMonitorID);
        this->elecManualAct->setEnabled(true);
        this->visionTemplateAct->setEnabled(true);
        this->portSetAct->setEnabled(true);
        this->productManagementAct->setEnabled(true);
        emit signal_enable_disable_threshold_button(true);
    }
}

void MainWindow::slot_create_vision_template()
{
    GDataFactory::get_factory()->set_system_status(Calibrate_Status);
    GDataFactory::get_vision_template_wgt()->show();
}

void MainWindow::slot_elec_manual_control()
{
    GDataFactory::get_factory()->set_system_status(Manual_Status);
    GDataFactory::get_elec_manual_wgt()->show();
}

void MainWindow::slot_para_set()
{
    GDataFactory::get_para_config_wgt()->show();
}

void MainWindow::slot_proc_plc_notify(QByteArray data)
{
    QByteArray tmpData;
    tmpData.append(static_cast<char>(0x01));
    m_pData = tmpData;
    if(data.length() == 2)
    {
        if((data[0]&0xFF) == 0xC8)//D200
        {
            if((data[1] & 0xFF) == 0x01)//notify camera to take picture and scan code
            {
                GDataFactory::get_time_step_counter()->start();
                if(GDataFactory::get_factory()->get_use_camera_sign() == false)//use scanner
                {
                    if((GDataFactory::get_factory()->get_code_list().length() != 4)&&
                            (GDataFactory::get_factory()->get_code_list().length() != 5)&&
                            (GDataFactory::get_factory()->get_code_list().length() != 6))
                        m_pTimerID = startTimer(1000);
                    else
                    {
                        QString strStyle;
                        QString tmpFullSN = "";
                        if(GDataFactory::get_factory()->get_code_list().length() == 4)
                        {
                            strStyle = GDataFactory::get_factory()->get_product_style(GDataFactory::get_factory()->get_code_list()[1]);
                            GDataFactory::get_factory()->set_current_bar_code(GDataFactory::get_factory()->get_code_list()[1].append("-").append(strStyle));
                        }
                        else if((GDataFactory::get_factory()->get_code_list().length() == 5)||
                                (GDataFactory::get_factory()->get_code_list().length() == 6)||
                                (GDataFactory::get_factory()->get_code_list().length() == 7))
                        {
                            strStyle = GDataFactory::get_factory()->get_product_style(GDataFactory::get_factory()->get_code_list()[0]);
                            GDataFactory::get_factory()->set_current_bar_code(GDataFactory::get_factory()->get_code_list()[0].append("-").append(strStyle));
                        }

                        for(int i=0;i<GDataFactory::get_factory()->get_code_list().length();i++)
                        {
                            tmpFullSN.append(GDataFactory::get_factory()->get_code_list()[i]);
                            if(i<GDataFactory::get_factory()->get_code_list().length()-1)
                                tmpFullSN.append("-");
                        }
                        GDataFactory::get_factory()->set_current_full_sn(tmpFullSN);

                        if(GDataFactory::get_factory()->get_code_list().length() == 4)
                            GDataFactory::get_factory()->set_current_second_section_code(GDataFactory::get_factory()->get_code_list()[1]);
                        else if((GDataFactory::get_factory()->get_code_list().length() == 5)||
                                (GDataFactory::get_factory()->get_code_list().length() == 6))
                            GDataFactory::get_factory()->set_current_second_section_code(GDataFactory::get_factory()->get_code_list()[0]);

                        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W201,ADDRESS_W201_00,tmpData.length(),tmpData);
                        //1、get product style by sn
                        //2、get sender's power by product style
                        //3、set the sender's power

                        GDataFactory::get_factory()->set_sender_power(strStyle);
                        GDataFactory::get_factory()->read_serial_number_xray(5);//set sender's vp
                        QThread::msleep(200);
                        GDataFactory::get_factory()->read_serial_number_xray(6);//set sender's cp
//                        QThread::msleep(500);
                        //when scan code SUCCESS,notify PLC take picture over
                        m_pTimerID = startTimer(1000);
                        GDataFactory::get_factory()->clear_code_list();
                    }
                }
                else//use camera
                {
                    GDataFactory::get_time_step_counter()->start();
                    //add vision code here
                    bool foundSign = false;
                    cv::Mat image,image1;
                    GDataFactory::get_camera_interface()->StartGrabbing();
                    GDataFactory::get_camera_interface()->SoftwareTriggerOnce();
                    QEventLoop loop;
                    QTimer::singleShot(300, &loop, SLOT(quit()));
                    loop.exec();
                    image1 = image = GDataFactory::get_camera_interface()->GetImage();
                    GDataFactory::get_camera_interface()->StopGrabbing();

                    if (!image.empty() && image.data)
                    {
                        Mat src2;
                        Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
                        filter2D(image, src2,CV_8UC3, kernel);

                        GDataFactory::get_factory()->scan_camera_mat_to_pixmap(image);
                        vector<string> ss;
                        vector<RotatedRect> pp;
                        image = src2;
                        GDataFactory::get_bar_code_decoding()->decode_h(image,ss,pp);

                        vector<vector<string>> allCodeSegs;
                        for(int i=0;i<ss.size();i++)
                        {
    //                        QLOG_WARN()<<QString("%1").fromStdString(ss[i]);
                            allCodeSegs.push_back(split_string(ss[i],"-"));
                        }

                        vector<string> currentPCode;
                        for(int i=0;i<allCodeSegs.size();i++)
                        {
                            if(allCodeSegs[i].size() != 4)
                                continue;
                            else
                            {
                                currentPCode = allCodeSegs[i];
                                foundSign = true;
                            }
                        }

                        if(foundSign)
                        {
                            QString strStyle = GDataFactory::get_factory()->get_product_style(QString::fromStdString(currentPCode[1]));
                            GDataFactory::get_factory()->set_current_bar_code(QString::fromStdString(currentPCode[1]).append("-").append(strStyle));
                            QString tmpFullSN = "";
                            for(int i=0;i<4;i++)
                            {
                                tmpFullSN.append(QString::fromStdString(currentPCode[i]));
                                if(i<3)
                                    tmpFullSN.append("-");
                            }
                            GDataFactory::get_factory()->set_current_full_sn(tmpFullSN);

                            GDataFactory::get_factory()->set_current_second_section_code(QString::fromStdString(currentPCode[1]));
                            GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W201,ADDRESS_W201_00,tmpData.length(),tmpData);
                            //1、get product style by sn
                            //2、get sender's power by product style
                            //3、set the sender's power

                            GDataFactory::get_factory()->set_sender_power(strStyle);
                            GDataFactory::get_factory()->read_serial_number_xray(5);//set sender's vp
                            QThread::msleep(200);
                            GDataFactory::get_factory()->read_serial_number_xray(6);//set sender's cp
    //                        QThread::msleep(500);
                            //when scan code SUCCESS,notify PLC take picture over
                            m_pTimerID = startTimer(1000);
                        }
                        else
                        {
                            //read QRCode use opencv
                            Mat s = image1.clone();
//                            cvtColor(image1, image1, COLOR_BGR2GRAY);
                            threshold(image1, image1, 160, 255, THRESH_BINARY);
                            Mat kernel = Mat::ones(Size(55, 55), CV_8UC1);
                            morphologyEx(image1, image1, MORPH_CLOSE, kernel);
                            Mat kernel2 = Mat::ones(Size(25, 25), CV_8UC1);
                            morphologyEx(image1, image1, MORPH_OPEN, kernel2);
                            vector<vector<Point>> contours;
                            vector<Vec4i> hierachy;
                            findContours(image1, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
                            vector<Mat> vv;
                            for (int i = 0; i < contours.size(); i++)
                            {
                                int area = contourArea(contours[i]);
                                if (area < 400) continue;
                                Rect r = boundingRect(contours[i]);

                                rectangle(s, r, Scalar(0, 0, 255), 4);
                                Mat patch = s.clone()(r);
                                vv.push_back(patch);
                            }
                            QString cPath = QApplication::applicationDirPath().append("/QRConfig/");
                            GDataFactory::get_AlgoQRCode()->initModel(cPath.toStdString());
                            vector<string> v;
                            for (int i = 0; i < vv.size(); i++)
                            {
                                Mat m = vv[i];
                                vector<string> tmpv;
                                GDataFactory::get_AlgoQRCode()->detectQRCode(m, tmpv);
                                for(int j=0;j<tmpv.size();j++)
                                    v.push_back(tmpv[j]);
                            }
                            vector<string> fullC;
                            //read QRCode use opencv

                            //use halcon
//                            vector<string> v = GDataFactory::get_factory()->halcon_QR_code(GDataFactory::get_factory()->mat_to_hobject(image1));
                            for(int i=0;i<v.size();i++)
                            {
                                fullC = split_string(v[i],"-");
                                if((fullC.size() == 5)||(fullC.size() == 6)||(fullC.size() == 7))
                                {
                                    foundSign = true;
                                    string am = v[i];
                                    GDataFactory::get_factory()->set_current_full_sn(QString::fromStdString(am));
                                    break;
                                }
                            }

                            if(foundSign)
                            {
                                QLOG_WARN()<<"QR step5";
                                QString strStyle = GDataFactory::get_factory()->get_product_style(QString::fromStdString(fullC[0]));
                                GDataFactory::get_factory()->set_current_bar_code(QString::fromStdString(fullC[0]).append("-").append(strStyle));

                                GDataFactory::get_factory()->set_current_second_section_code(QString::fromStdString(fullC[0]));
                                GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W201,ADDRESS_W201_00,tmpData.length(),tmpData);
                                //1、get product style by sn
                                //2、get sender's power by product style
                                //3、set the sender's power

                                GDataFactory::get_factory()->set_sender_power(strStyle);
                                GDataFactory::get_factory()->read_serial_number_xray(5);//set sender's vp
                                QThread::msleep(200);
                                GDataFactory::get_factory()->read_serial_number_xray(6);//set sender's cp
        //                        QThread::msleep(500);
                                //when scan code SUCCESS,notify PLC take picture over
                                m_pTimerID = startTimer(1000);
                            }
                            else
                            {
                                QLOG_ERROR()<<u8"读码失败！继续读码或请更换料盘!";
                                m_pTimerID = startTimer(1000);
                            }

                        }
                    }
                    else
                    {
                        QLOG_WARN()<<"camera take picture failed! take picture again";
                        m_pTimerID = startTimer(1000);
                    }
                }

            }
            else
            {
                GDataFactory::get_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D202,0x02,nullptr);
            }
        }
        else if((data[0]&0xFF) == 0xCA)//D202
        {
            if((data[1]&0xFF) == 0x01)
            {
                //add sender code here
                GDataFactory::get_factory()->read_serial_number_xray(3);
//                QThread::msleep(500);
                GDataFactory::get_factory()->get_xray_image();
//                QThread::msleep(500);
                GDataFactory::get_factory()->read_serial_number_xray(7);
                //add sender code here

                m_pTimerTakePictureOverID = startTimer(200);
            }
            else
            {
                m_pTimerID = startTimer(1000);
            }
        }

    }
}
static int a = 11;
static int b = 12;
#define test_add() \
if(a == 11){\
    a = b;\
}

void MainWindow::slot_product_management()
{
//    int i = 10;
//    int j = 11;
//    if(i==j?1:0)
//        QLOG_INFO()<<"1";
//    else
//        QLOG_INFO()<<"2";
//    test_add();
//    QLOG_WARN()<<a;
    GDataFactory::get_product_manage_wgt()->show();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
//    GDataFactory::get_factory()->release_receiver_res();
}
