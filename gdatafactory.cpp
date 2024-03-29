#include "gdatafactory.h"
#include "QsLog.h"
#include <QApplication>
#include <QHostInfo>
#include <QTime>
#include "TemplateBasedMethod.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QHttpPart>
#include <QUuid>
#include <QNetworkReply>
#include <QFileInfo>
#include <QJsonParseError>
#include "x-ray-pre.h"
#include <functional>
#include <thread>
#include <future>
#include <vector>
#include <QLibrary>
#include <iostream>
using std::vector;
using namespace std;
TopWidget* GDataFactory::m_pTopWgt = nullptr;
MainWindow* GDataFactory::m_pMainWindow = nullptr;
CentralWgt* GDataFactory::m_pCentralWgt = nullptr;
AboutWgt* GDataFactory::m_pAboutWgt = nullptr;
VisionTemplateWgt* GDataFactory::m_pVisionTemplateWgt = nullptr;
ZoomMoveObj* GDataFactory::m_pZoomMoveObj = nullptr;
GDataFactory* GDataFactory::m_pDataFactory = nullptr;
TemplateConfigWgt* GDataFactory::m_pTemplateConfigWgt = nullptr;
PixmapWgt* GDataFactory::m_pPixmapWgt = nullptr;
ProcessInfoWgt* GDataFactory::m_pProcessInfoWgt = nullptr;
ProcessPixmapWgt* GDataFactory::m_pProcessPixmapWgt = nullptr;
CDetector* GDataFactory::m_pDetectorObj = nullptr;
ElecManualWgt* GDataFactory::m_pElecManualWgt = nullptr;
UdpService* GDataFactory::m_pUdpService = nullptr;
ParaConfigWgt* GDataFactory::m_pParaConfigWgt = nullptr;
WelcomeDlg* GDataFactory::m_pWelcomeDlg = nullptr;
MonitorThread* GDataFactory::m_pMonitorThread = nullptr;
DynamicCircleWgt* GDataFactory::m_pDynamicCircleWgt = nullptr;
EquipInfoWgt* GDataFactory::m_pEquipInfoWgt = nullptr;
ProductStyleDlg* GDataFactory::m_pProductStyleDlg = nullptr;
CameraInterface* GDataFactory::m_pCameraInterface =nullptr;
BarCodeDecoding* GDataFactory::m_pBarCodeDecoding =nullptr;
DMDBAccessInterface* GDataFactory::m_pDMDBAccess = nullptr;
TestWindow* GDataFactory::m_pTestWindow = nullptr;
QTime* GDataFactory::m_pTimeStepCounter = nullptr;
ProductManageWgt* GDataFactory::m_pProductManageWgt = nullptr;
WaitCountWgt* GDataFactory::m_pWaitCountWgt = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManager = nullptr;
AlgoQRCode* GDataFactory::m_pAlgoQRCode = nullptr;
LoginDialog* GDataFactory::m_pLoginDialog = nullptr;

QImage GDataFactory::mat_to_qimage(const int saveSign,const Mat &cvImage)
{
    vector<uchar> imgBuf;
    imencode(".bmp", cvImage, imgBuf);

    QByteArray baImg((char*)imgBuf.data(), static_cast<int>(imgBuf.size()));
    QImage image;
    image.loadFromData(baImg, "BMP");
    QDateTime current_date_time =QDateTime::currentDateTime();
    if(get_system_status() == Run_Status)
    {
        QString current_date =current_date_time.toString("yyyyMMddhhmmss");
        QString imageName = current_date +".jpg";
        if(saveSign == 0)//the image of camera do not save
            ;
        else
//            image.save(imageName);
            ;
    }
    else {
        QString current_path ="e:/Initial/";
        current_path.append(GDataFactory::get_current_product_style());
        QString imageName = current_path +".jpg";
        image.save(imageName);
    }

    return image;
}

QPixmap GDataFactory::mat_to_pixmap(const int saveSign,const Mat &cvImage)
{
    return QPixmap::fromImage(mat_to_qimage(saveSign,cvImage));
}

void GDataFactory::set_sender_power(QString strStyle)
{
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
            arg(constProductStylePowerTable).arg(strStyle);
    QSqlQuery queryResult;
    if(get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            m_pCurrentVoltage = queryResult.value(1).toString();
            m_pCurrentCurrent = queryResult.value(2).toString();
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
}

void GDataFactory::get_step_and_threshold_run(float &step, float &threshold)
{
    QLOG_WARN()<<"the current product-style is:"<<m_pCurrentProductStyle;
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
            arg(constProductStylePowerTable).arg(m_pCurrentProductStyle);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        while(queryResult.next())
        {
            float a = queryResult.value(4).toFloat();
            emit signal_send_current_threshold(QString("%1").arg(a));
            step = queryResult.value(3).toFloat();
            threshold = queryResult.value(4).toFloat();
//            QLOG_WARN()<<"the vision step in DB is:"<<step;
//            QLOG_WARN()<<"the vision threshold in DB is:"<<threshold;

        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

void GDataFactory::submit_msg_to_DB(int amount)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString strSql = QString("select max(id) from public.%1").
            arg(constCountResultTable);

    int id = -1;
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }

    strSql = QString("insert into public.%1 (id,\"SN\",\"Product_Amount\",\"Count_Date\",\"Count_Time\") "
                     "values(%2,'%3','%4','%5','%6')").
                arg(constCountResultTable).arg(id+1).arg(m_pCurrentSecondSectionCode).arg(QString::number(amount)).
                arg(date.toString("yyyy/MM/dd")).
                arg(time.toString("HH:mm:ss"));

    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
        QLOG_INFO()<<"insert into DB SUCCESS!";
    else {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
//    vector<int> vecT = {1,2,3,4,5,6};
//    std::packaged_task<int(const vector<int>&)> pl([](const vector<int>& vec)
//    {
//        return *(std::max_element(vec.begin(),vec.end()));
//    });

//    static int maxID=-1;
//    std::packaged_task<void(std::shared_future<int>&)> res([](std::shared_future<int>& sfu)
//    {
//        sfu.wait();
//        maxID = sfu.get();
//     });

//    std::shared_future<int> s_fu(pl.get_future());
//    if(pl.valid())
//    {
//        std::thread t1(ref(res), ref(s_fu));
//        QThread::msleep(10);
//        std::thread(ref(pl), ref(vecT)).join();
//        t1.join();
//    }
}

static cv::Mat currentFullImage;
static cv::Mat tempMat;
static QString normalProductStyle;
static float m_pVisionStep;
static float m_pVisionThreshold;

void GDataFactory::count_product()
{
    QLOG_WARN()<<u8"--- 开始点料 ---";

    if(m_pCurrentProductStyle == "")
    {
        QByteArray tmpData;
        tmpData.append(static_cast<char>(0x01));
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_00,tmpData.length(),tmpData);
        QThread::msleep(200);
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_01,tmpData.length(),tmpData);
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"当前物料号不存在，请录入物料");
        GDataFactory::get_main_window()->slot_start_run();
        return;
    }

    normalProductStyle = m_pCurrentProductStyle;
    QString tmpProductStyle = m_pCurrentProductStyle;
    tmpProductStyle.prepend(u8"e:/template/");
    tmpProductStyle.append(".png");
    QLOG_WARN()<<u8"当前模板为:"<<tmpProductStyle;
    QFile file(tmpProductStyle);
    if(file.exists())
        ;
    else
    {
        QByteArray tmpData;
        tmpData.append(static_cast<char>(0x01));
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_00,tmpData.length(),tmpData);
        QThread::msleep(200);
        GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_W203,ADDRESS_W203_01,tmpData.length(),tmpData);
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"模板文件不存在，请制作模板文件");
//        QLOG_ERROR()<<u8"模板文件不存在，请制作模板文件";
        GDataFactory::get_main_window()->slot_start_run();
        return;
    }

    tempMat = cv::imread(tmpProductStyle.toStdString());
    currentFullImage = get_current_image_mat();

    cvtColor(currentFullImage,currentFullImage,COLOR_GRAY2BGR);
    if(currentFullImage.data && tempMat.data)
    {
        get_step_and_threshold_run(m_pVisionStep,m_pVisionThreshold);
        GDataFactory::get_wait_count_wgt()->show();
        m_pNormalCountThread->start();
    }
}

void NormalCountThread::run()
{
    while(1)
    {
        QTime timeS;
        timeS.start();
        TemplateBasedMethod t;

        xray_pre(currentFullImage);
        t.setImage(currentFullImage);
        t.setTemplateImage(tempMat);
        t.setMaxMatch(20000);
        if((m_pVisionStep>0.0)&&(m_pVisionStep < 1.0))
        {
            t.setAngleStep(m_pVisionStep);
        }
        else
            t.setAngleStep(0.5);

        if((m_pVisionStep>0.0)&&(m_pVisionStep < 1.0))
        {
            t.setThreshold(m_pVisionThreshold);
        }
        else {
            t.setThreshold(0.75);
        }
        t.CreateNCCTemplate();
        t.TemplateMatchWithNCC();

        UnitInfo info = t.getCheckResult();
        QLOG_WARN()<<u8"当前物料数量为:"<<info.units_num;
        emit signal_normal_count_over(info.units_num,(double)(timeS.elapsed())/1000.0);
        break;
    }
}

void GDataFactory::slot_normal_count_over(int amount, double counttime)
{
    GDataFactory::get_wait_count_wgt()->hide();
    m_pNormalCountThread->terminate();
    QLOG_INFO()<<"terminate the normal test count thread!";
    get_main_window()->set_count_sign(true);
    emit signal_notify_time_step(QString("%1").arg(GDataFactory::get_time_step_counter()->elapsed()/1000.0));
    emit signal_notify_count_result(amount,counttime);
    //post msg to MES
    submit_msg_to_mes(m_pCurrentFullSN,QString("%1").arg(amount));
    submit_msg_to_DB(amount);
}

void GDataFactory::submit_msg_to_mes(QString currentSN, QString productQuantity)
{
    QLOG_WARN()<<"productQuantity is:"<<productQuantity;
    //notify printer to print the SN and the product quantity
    if(get_config_para("USE_PRINTER") == "0")
        ;
    else {
//        typedef int (*TSCabout)();
//        typedef int (*TSCopenport)(char*);
//        typedef int (*TSCsendcommand)(char*);
//        typedef int (*TSCcloseport)();

//        QLibrary tscdll("C:\\TSCLIB.dll");
//        if(tscdll.load())
//        {   //add about() function
//             TSCopenport openport=(TSCopenport)tscdll.resolve("openport");    //add about() function
//             TSCcloseport closeport=(TSCcloseport)tscdll.resolve("closeport");    //add closeport() function
//             TSCsendcommand sendcommand=(TSCsendcommand)tscdll.resolve("sendcommand");    //add about() function



//             openport("TSC TTP-244 Pro");
//    //         sendcommand("SET TEAR ON");
//             sendcommand("SIZE 63 mm, 33 mm");
//             sendcommand("DIRECTION 1");
//             sendcommand("CLS");
////             currentSN.prepend("BARCODE 16,56, \"128\",70,1,0,1,1, \"");
////             currentSN.append("\"");
////             sendcommand(currentSN.toLatin1().data());
//             productQuantity.prepend("BARCODE 96,152, \"128\",70,1,0,4,4, \"");
//             productQuantity.append("\"");
//             sendcommand(productQuantity.toLatin1().data());
//    //             sendcommand("BARCODE 16,168, \"128\",70,1,0,2,2, \"2549\"");
//             sendcommand("PRINT 1");
//             closeport();
        ;
        }
    //notify printer to print the SN and the product quantity

    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(get_config_para("MES_PORT"));
    tmpurl.append("/InfoWebs.asmx/InfoApi");
    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));

    QString reType = QString::fromUtf8("application/x-www-form-urlencoded");
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant(reType));

    QJsonObject reqJson;
    reqJson.insert("TYPE","X");
    reqJson.insert("WORKSTATION",u8"TLQ");
    reqJson.insert("EMP",GDataFactory::get_factory()->get_user_account());
    reqJson.insert("SN",currentSN);
    reqJson.insert("QTY",productQuantity);
    QJsonDocument doc_data(reqJson);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<u8"上传Mes信息为:"<<request_data;

//    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    QString rInfo = "info=";
    rInfo.append(request_data);
    reply = get_net_work_access_manager()->post(*request,rInfo.toUtf8());
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    if(reply->isFinished())
    {
        QByteArray responseByte = reply->readAll();
        QString tmpData = QString::fromUtf8(responseByte);
        int index1 = tmpData.indexOf("{");
        int index2 = tmpData.indexOf("}");
        int indexLength = index2 - index1 + 1;
        tmpData = tmpData.mid(index1,indexLength);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(tmpData.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();
        if(jsonObject.contains("RES"))
        {
            if(jsonObject.value(QStringLiteral("RES")).toString() == "OK")
                QLOG_INFO()<<"submit product info to mes SUCCESS!";
            else {
                QLOG_WARN()<<u8"点料信息上传Mes失败!";
                QLOG_WARN()<<u8"Mes 返回 RES:"<<jsonObject.value(QStringLiteral("RES")).toString();
                QLOG_WARN()<<u8"上传失败原因:"<<jsonObject.value(QStringLiteral("RESMSG")).toString();
            }
        }
        else
            QLOG_WARN()<<"MES reply is ABNORMAL!";
    }
    else
    {
        QLOG_WARN()<<"MES reply is TIMEOUT!";;
    }
}

void GDataFactory::scan_camera_mat_to_pixmap(const Mat& cvImage)
{
    if(get_system_status() == Run_Status)
        emit signal_spread_pixmap_to_ui(mat_to_pixmap(0,cvImage));
}

QMap<QString,QString> GDataFactory::get_product_info()
{
    QMap<QString,QString> m_pAllProductInfo;
    QString strSql = QString("select * from public.%1").
            arg(constProductStyleMapTable);
    QSqlQuery queryResult;
    if(get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        while(queryResult.next())
        {
            m_pAllProductInfo.insert(queryResult.value(0).toString(),queryResult.value(1).toString());
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
    return m_pAllProductInfo;
}

GDataFactory::GDataFactory()
{
    pComm = nullptr;
    pCommCode = nullptr;
    m_pCurrentUserAccount = "";
    m_pCurrentVoltage = "400";
    m_pCurrentCurrent = "6000";
    m_pUseCamera = true;
    m_pNormalCountThread = new NormalCountThread();
    connect(m_pNormalCountThread,SIGNAL(signal_normal_count_over(int,double)),this,SLOT(slot_normal_count_over(int,double)));
    connect(this,SIGNAL(signal_start_to_count()),this,SLOT(slot_start_to_count()));
}

void GDataFactory::connections_initialization()
{
    //init class obj
    get_udp_service();
    get_elec_manual_wgt();
    get_welcome_dlg();
    get_template_config_wgt();
    //init class obj
    connect(this,SIGNAL(signal_notify_time_step(QString)),get_process_info_wgt(),SLOT(slot_rev_count_step(QString)));
    connect(this,SIGNAL(signal_notify_bar_code(QString)),get_process_info_wgt(),SLOT(slot_rev_bar_code(QString)));
    connect(this,SIGNAL(signal_notify_count_result(int,double)),get_process_info_wgt(),SLOT(slot_rev_count_result(int,double)));
    connect(this,SIGNAL(signal_spread_pixmap_to_ui(QPixmap)),get_process_pixmap_wgt(),SLOT(slot_receive_pixmap(QPixmap)));
    connect(this,SIGNAL(signal_spread_pixmap_to_ui(QPixmap)),get_zoom_obj(),SLOT(slot_rev_pixmap(QPixmap)));
    connect(get_elec_manual_wgt(),SIGNAL(signal_process_serial_info(int,QString)),get_equip_info_wgt(),SLOT(slot_process_info(int,QString)));
    connect(get_monitor_thread(),SIGNAL(signal_send_receiver_info(int,QString)),get_equip_info_wgt(),SLOT(slot_process_info(int,QString)));
    connect(get_zoom_obj(),SIGNAL(signal_transfer_pixmap(QPixmap)),get_pixmap_wgt(),SLOT(slot_receive_pixmap(QPixmap)));
    connect(get_template_config_wgt(),SIGNAL(signal_notify_take_picture()),get_zoom_obj(),SLOT(slot_take_image()));
    connect(this,SIGNAL(signal_serial_data(QByteArray)),get_elec_manual_wgt(),SLOT(slot_process_serial_data(QByteArray)));
    connect(get_udp_service(),SIGNAL(signal_notify_app_by_plc(QByteArray)),get_main_window(),SLOT(slot_proc_plc_notify(QByteArray)));
    connect(get_udp_service(),SIGNAL(signal_notify_app_by_manual(QByteArray)),get_elec_manual_wgt(),SLOT(slot_process_udp_data(QByteArray)));
    connect(get_main_window(),SIGNAL(signal_enable_disable_threshold_button(bool)),get_process_info_wgt(),SLOT(slot_rev_button_control_sign(bool)));
    connect(this,SIGNAL(signal_send_current_threshold(QString)),get_process_info_wgt(),SLOT(slot_rev_current_threshold(QString)));
}

int GDataFactory::initialize_xray()
{
    if(get_detector_obj()->LoadIRayLibrary() == Err_OK)
        QLOG_INFO()<<"XRay library loaded SUCCESS!";
    else
    {
        QLOG_WARN()<<"XRay library loaded FAILED!";
        return -1;
    }

    QString binDir=QApplication::applicationDirPath();
    binDir = binDir + "/workhome";
    int ret = get_detector_obj()->Create(binDir.toLatin1(), SDKCallbackHandler);
    if (Err_OK != ret)
    {
        QLOG_WARN()<<"error:"<<get_detector_obj()->GetErrorInfo(ret).c_str();
        return ret;
    }
    else
    {
        QLOG_INFO()<<"XRay create instance SUCCESS!";
        QLOG_WARN()<<"connect XRay......";
        ret = get_detector_obj()->SyncInvoke(Cmd_Connect,30000);
        if(ret != Err_OK)
        {
            QLOG_WARN()<<"connect XRay device FAILED!";
            return ret;
        }
        else {
            QLOG_INFO()<<"connect XRay device SUCCESS!";
            return ret;
        }
    }
}

void GDataFactory::get_xray_image()
{
    get_detector_obj()->SetAttr(Attr_UROM_FluroSync_W, Enm_FluroSync_SyncOut);
    get_detector_obj()->SyncInvoke(Cmd_WriteUserRAM, 4000);
    int nExposeWindowTime = 1000;
    int nTimeOut = nExposeWindowTime + 500;
    get_detector_obj()->SetAttr(Cfg_ClearAcqParam_DelayTime, nExposeWindowTime);
    get_detector_obj()->SyncInvoke(Cmd_ClearAcq, nTimeOut);
}

void GDataFactory::load_json_config(char *filename)
{
    m_pConfigFile.setFileName(QString(filename));
    m_pConfigFileName=QString(filename);
    QString JsonValue;
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QLOG_INFO()<<"load json config file success!";
        JsonValue = m_pConfigFile.readAll();
        m_pConfigFile.close();

        QJsonParseError parseJsonErr;
        m_pJsonDocument = QJsonDocument::fromJson(JsonValue.toUtf8(),&parseJsonErr);
        if(!(parseJsonErr.error == QJsonParseError::NoError))
        {
            QLOG_INFO()<<"parse json file failed!";
            return;
        }
        else {
            QLOG_INFO()<<"parse json file success!";
        }

        m_pRootObj = m_pJsonDocument.object();

        if(m_pRootObj.contains("SenderVoltage"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("SenderVoltage"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue tmpAValue = array.at(i);
                    if(tmpAValue.isObject())
                    {
                        QJsonObject aObj = tmpAValue.toObject();
                        QString tmpAStr = "Voltage"+QString::number(i+1);
                        m_pVoltageList.push_back(aObj.value(tmpAStr).toString());
                    }
                }
            }
        }

        if(m_pRootObj.contains("SenderCurrent"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("SenderCurrent"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue tmpAValue = array.at(i);
                    if(tmpAValue.isObject())
                    {
                        QJsonObject aObj = tmpAValue.toObject();
                        QString tmpAStr = "Current"+QString::number(i+1);
                        m_pCurrentList.push_back(aObj.value(tmpAStr).toString());
                    }
                }
            }
        }

        if(m_pRootObj.contains("IPConfigs"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("IPConfigs"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    ConfigInfo.insert("MES_IP",icon["MES_IP"].toString());
                    ConfigInfo.insert("COM_PORT",icon["COM_PORT"].toString());
                    ConfigInfo.insert("COM_PORT_SCANNER",icon["COM_PORT_SCANNER"].toString());
                    ConfigInfo.insert("PLC_IP",icon["PLC_IP"].toString());
                }
            }
        }

        if(m_pRootObj.contains("PortConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PortConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("MES_PORT"))
                    ConfigInfo.insert("MES_PORT",obj.value("MES_PORT").toString());
                if(obj.contains("COM_BARD_RATE"))
                    ConfigInfo.insert("COM_BARD_RATE",obj.value("COM_BARD_RATE").toString());
                if(obj.contains("COM_BARD_RATE_SCANNER"))
                    ConfigInfo.insert("COM_BARD_RATE_SCANNER",obj.value("COM_BARD_RATE_SCANNER").toString());
                if(obj.contains("PLC_PORT"))
                    ConfigInfo.insert("PLC_PORT",obj.value("PLC_PORT").toString());
            }
        }

        if(m_pRootObj.contains("UsrPermissions"))
        {
            QJsonValue usrValue = m_pRootObj.value(QStringLiteral("UsrPermissions"));
            if(usrValue.isObject())
            {
                QJsonObject obj = usrValue.toObject();
                if(obj.contains("AUTHORIZATION"))
                    ConfigInfo.insert("AUTHORIZATION",obj.value("AUTHORIZATION").toString());
                if(obj.contains("LOG_LEVEL"))
                    ConfigInfo.insert("LOG_LEVEL",obj.value("LOG_LEVEL").toString());
                if(obj.contains("IMAGE_STRETCH_FILL"))
                    ConfigInfo.insert("IMAGE_STRETCH_FILL",obj.value("IMAGE_STRETCH_FILL").toString());
                if(obj.contains("USE_CAMERA"))
                    ConfigInfo.insert("USE_CAMERA",obj.value("USE_CAMERA").toString());
                if(obj.contains("USE_PRINTER"))
                    ConfigInfo.insert("USE_PRINTER",obj.value("USE_PRINTER").toString());
            }
        }
    }
    else
    {
        QLOG_INFO()<<"load json config file failed!";
        return;
    }
    m_pConfigFile.flush();
}

QString GDataFactory::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void GDataFactory::set_config_para(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    if(keyname == "COM_BARD_RATE" ||
            keyname == "PLC_PORT")
    {
        if(m_pRootObj.contains("PortConfigs") && m_pRootObj["PortConfigs"].isObject())
        {
            QJsonValueRef refValue = m_pRootObj.find("PortConfigs").value();
            QJsonObject obj = refValue.toObject();
            obj[keyname] = keyvalue;
            refValue = obj;
        }
    }

    if((keyname == "COM_PORT") ||
            (keyname == "PLC_IP"))
    {
        QJsonValueRef refValue = m_pRootObj.find("IPConfigs").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[0];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    m_pJsonDocument.setObject(m_pRootObj);
}

quint8 GDataFactory::process_ip_string(const QString ipstr)
{
    int nSize = ipstr.size();
    int nCount = 0;
    QString mData;
    mData.clear();
    QChar mChar;
    for(int idx = 0; idx<nSize; idx++)
    {
        mChar = ipstr[idx];
        if(mChar == '.')
        {
            nCount++;
        }
        else if(mChar != '.' && nCount == 3 && (mChar >= '0' || mChar <= '9'))
        {
            mData.append(mChar);
        }
    }
    return static_cast<quint8>(mData.toInt());
}

quint8 GDataFactory::get_last_ip(const int id)
{
    if (id == 0)
    {
        QString localHostName = QHostInfo::localHostName();
        QString localIP = "";
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress addr,info.addresses())
            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                localIP = addr.toString();
            }
        return process_ip_string(localIP);
    }
    else
    {
        return process_ip_string(get_config_para("PLC_IP"));
    }
}

static int s_nExpWindow = 0;
static IRayTimer s_timer;
void TimeProc(int uTimerID)
{
    s_nExpWindow -= 1;
    if (0 == s_nExpWindow)
    {
        s_timer.Close();
        return;
    }
    QLOG_INFO()<<QString("Please expose in %1s").arg(s_nExpWindow);
}

void GDataFactory::SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel, const char *pszMsg, int nParam1, int nParam2, int nPtrParamLen, void *pParam)
{
    get_detector_obj()->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);
    switch (nEventID)
    {
    case Evt_Exp_Enable:
        QLOG_INFO()<<"Prepare to expose";
        s_timer.Init(TimeProc, 1000);
        s_nExpWindow = nParam1 / 1000;
        QLOG_INFO()<<QString("Please expose in %1s").arg(s_nExpWindow);
        break;
    case Evt_Image:
        QLOG_INFO()<<"Got Image";
    {
        //must make deep copies of pParam
        IRayImage* pImg = (IRayImage*)pParam;
        unsigned short* pImageData = pImg->pData;
        get_factory()->raw_to_mat(pImageData,pImg->nWidth,pImg->nHeight);
    }
        break;
    default:
        break;
    }
}

void GDataFactory::slot_start_to_count()
{
    count_product();
}

void GDataFactory::slot_rev_serial_number(const QByteArray &data)
{
    QLOG_INFO()<<"the response of serial port is:"<<data;
    emit signal_serial_data(data);

    QString str = QString::fromUtf8(data);
    if(GDataFactory::get_factory()->get_system_status() == Init_Status)
    {
        if(str.contains("2000"))
        {
//            emit signal_process_serial_info(1,str);
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(1);
        }
        else if(str.contains("WDOG0"))
        {
//            emit signal_process_serial_info(1,str);
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(2);
        }
        else if(str.contains("CLR"))
        {
//            emit signal_process_serial_info(1,str);
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(5);
        }
        else if(str.contains("VP"))
        {
//            emit signal_process_serial_info(1,str);
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,str);
            GDataFactory::get_factory()->read_serial_number_xray(6);
        }
        else if(str.contains("CP"))
        {
//            emit signal_process_serial_info(1,str);
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,str);
//            emit signal_process_serial_info(1,u8"发射源初始化完成!");
            GDataFactory::get_equip_info_wgt()->slot_process_info(1,u8"发射源初始化完成!");
            GDataFactory::get_factory()->set_system_status(Manual_Status);
        }
    }
}

void GDataFactory::save_config_file()
{
    m_pConfigFile.setFileName(m_pConfigFileName);
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_pConfigFile.resize(0);//this sentence is essential,or the file will be abnormal---20211123lilei
        m_pJsonDocument.setObject(m_pRootObj);
        m_pConfigFile.seek(0);
        m_pConfigFile.write(m_pJsonDocument.toJson());
        m_pConfigFile.close();
        m_pConfigFile.flush();
    }
}

void GDataFactory::read_serial_number_xray(int functioncode)
{
    if(pComm)
    {
        delete pComm;
        pComm = nullptr;
    }

    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = ConfigInfo["COM_PORT"];
        QString portRate = ConfigInfo["COM_BARD_RATE"];
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));

        QList<QByteArray> datas;
        QByteArray aa;
        switch (functioncode) {
        case 0://FREV
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x46));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x56));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"FREV";
            break;
        case 1://watch dog
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x57));
            aa.append(static_cast<char>(0x44));
            aa.append(static_cast<char>(0x4F));
            aa.append(static_cast<char>(0x47));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"watch dog";
            break;
        case 2://clr
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x43));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"CLR";
            break;
        case 3://enable
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x4E));
            aa.append(static_cast<char>(0x42));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x31));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"ENABLE";
            break;
        case 4://on
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x53));
            aa.append(static_cast<char>(0x54));
            aa.append(static_cast<char>(0x41));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x54));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"ON";
            break;
        case 5://vp
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x56));
            aa.append(static_cast<char>(0x50));
            aa.append(static_cast<char>(0x30));
            if(m_pCurrentVoltage.length() == 3)
            {
                QByteArray aaa = m_pCurrentVoltage.toUtf8();
                for(int i=0;i<m_pCurrentVoltage.length();i++)
                    aa.append(static_cast<char>(int8_t(aaa.at(i))));
            }
            else
            {
                aa.append(static_cast<char>(0x34));
                aa.append(static_cast<char>(0x30));
                aa.append(static_cast<char>(0x30));
            }
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"VP";
            break;
        case 6://cp
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x43));
            aa.append(static_cast<char>(0x50));
            if(m_pCurrentCurrent.length() == 4)
            {
                QByteArray aaa = m_pCurrentCurrent.toUtf8();
                for(int i=0;i<m_pCurrentCurrent.length();i++)
                    aa.append(static_cast<char>(int8_t(aaa.at(i))));
            }
            else
            {
                QLOG_ERROR()<<"current current is:"<<m_pCurrentCurrent;
                aa.append(static_cast<char>(0x36));
                aa.append(static_cast<char>(0x30));
                aa.append(static_cast<char>(0x30));
                aa.append(static_cast<char>(0x30));
            }
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"CP";
            break;
        case 7://unable
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x4E));
            aa.append(static_cast<char>(0x42));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
//            QLOG_INFO()<<"UNABLE";
            break;
        default:
            break;
        }

        datas.append(aa);
        pComm->sendRequest_and_recvResponse(datas,500);
        QThread qThread;
        pComm->moveToThread(&qThread);
        pComm->run();
    }
}

void GDataFactory::read_product_code_number()
{
    if(pCommCode)
    {
        delete pCommCode;
        pCommCode = nullptr;
    }

    pCommCode = new DataCommSerialEx();
    connect(pCommCode,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_product_code_number(const QByteArray&)));
    if(pCommCode)
    {
        QString portName = ConfigInfo["COM_PORT_SCANNER"];
        QString portRate = ConfigInfo["COM_BARD_RATE_SCANNER"];
        pCommCode->init(tagSerialPortInfoEx(portName,portRate.toInt()));
        pCommCode->recvResponse_only();
        pCommCode->start();
    }
}

void GDataFactory::slot_rev_product_code_number(const QByteArray& data)
{
    QByteArray tmpArray;
//    QLOG_WARN()<<data;
    if(data.length()>=20)
    {
        if(data[0] == 0x02)
        {
            for(int i=1;i<data.length();i++)
            {
                if((data[i] == 0x0d) || (data[i] == 0x0a))
                    break;
                else {
                    tmpArray.append(data[i]);
                }
            }
        }
    }
    if(tmpArray.length() > 20)
        phase_bar_code(tmpArray);
}

QStringList GDataFactory::phase_bar_code(QByteArray ar)
{
    m_pListCodeFromScanner.clear();
    QString strBarFullCode = ar;
    int count = 0;
    for(int i=0;i<strBarFullCode.length();i++)
    {
        if(strBarFullCode[i] == '-')
            count++;
        else
            continue;
    }

    if((count == 3)||
            (count == 4)||
            (count == 5)||
            (count == 6))
        m_pListCodeFromScanner = strBarFullCode.split("-");
//    for(int i=0;i<m_pListCodeFromScanner.length();i++)
//        qDebug()<<m_pListCodeFromScanner[i];
    return m_pListCodeFromScanner;
}

void GDataFactory::stop_read_product_code()
{
    disconnect(pCommCode,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_product_code_number(const QByteArray&)));
    pCommCode->terminate();
    pCommCode->quit();
    pCommCode->closeSerialPort();
    if(pCommCode)
        delete pCommCode;
    pCommCode = nullptr;
}

QString GDataFactory::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

HObject GDataFactory::mat_to_hobject(const cv::Mat &image)
{
    HObject Hobj = HObject();
    int hgt = image.rows;
    int wid = image.cols;
    int i;
    if (image.type() == CV_8UC3)
    {
        vector<Mat> imgchannel;
        split(image, imgchannel);
        Mat imgB = imgchannel[0];
        Mat imgG = imgchannel[1];
        Mat imgR = imgchannel[2];
        uchar* dataR = new uchar[hgt*wid];
        uchar* dataG = new uchar[hgt*wid];
        uchar* dataB = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
        {
            memcpy(dataR + wid*i, imgR.data + imgR.step*i, wid);
            memcpy(dataG + wid*i, imgG.data + imgG.step*i, wid);
            memcpy(dataB + wid*i, imgB.data + imgB.step*i, wid);
        }
        GenImage3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
        delete[]dataR;
        delete[]dataG;
        delete[]dataB;
        dataR = NULL;
        dataG = NULL;
        dataB = NULL;
    }
    else if (image.type() == CV_8UC1)
    {
        uchar* data = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
            memcpy(data + wid*i, image.data + image.step*i, wid);
        GenImage1(&Hobj, "byte", wid, hgt, (Hlong)data);
        delete[] data;
        data = NULL;
    }
    return Hobj;
}

vector<string> GDataFactory::halcon_QR_code(const HObject ho_Image)
{
    HObject  ho_SymbolXLDs;

    // 定义扫码句柄和扫码结果句柄
    HTuple  hv_DataCodeHandle, hv_ResultHandles;
    //定义扫码结果
    HTuple  hv_DecodedDataStrings;

    CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &hv_DataCodeHandle);
    FindDataCode2d(ho_Image, &ho_SymbolXLDs, hv_DataCodeHandle, HTuple(), HTuple(),
                   &hv_ResultHandles, &hv_DecodedDataStrings);
    //创建输出条码值
    vector<string> codeList;
    for(int i=0;i<hv_DecodedDataStrings.Length();i++)
    {
        char* resultSN = new char[255];
        sprintf_s(resultSN, 255, "%s", hv_DecodedDataStrings[0].S().Text());
        string str = resultSN;
        std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
        str.erase(end_pos, str.end());//移除空格
        codeList.push_back(str);
    }
    return codeList;
}

void GDataFactory::raw_to_mat(unsigned short *pBuffer, const int nWidth, const int nHeight)
{
    cv::Mat out(cv::Size(nWidth, nHeight), CV_8UC1);
    for (int row = 0; row < nHeight; row++)
    {
        for (int col=0; col < nWidth; col++)
        {
            long index = row*nWidth + col;
            out.at<uchar>(row,col)= pBuffer[index]/256;
        }
    }
    m_pCurrentImage = out;

    if(m_pSystemStatus == Run_Status)
    {
        emit signal_spread_pixmap_to_ui(mat_to_pixmap(1,out));
        emit signal_start_to_count();
    }
    else {
        emit signal_spread_pixmap_to_ui(mat_to_pixmap(1,out));
    }
}

QString GDataFactory::get_product_style(QString strSN)
{
    QString strSql = QString("select * from public.%1 where \"Second_Section_Code\"='%2'").
            arg(constProductStyleMapTable).arg(strSN);
    QSqlQuery queryResult;
    QString strStyle="";
    if(get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            strStyle = queryResult.value(1).toString();
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }

    m_pCurrentProductStyle = strStyle;
    return strStyle;
}

QList<QString> GDataFactory::get_product_style_list()
{
    QList<QString> lstProductStyle;
    lstProductStyle.clear();

    QString strSql = QString("select * from public.%1").
            arg(constProductStylePowerTable);
    QSqlQuery queryResult;
    if(get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            lstProductStyle.push_back(queryResult.value(0).toString());
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
    return lstProductStyle;
}

