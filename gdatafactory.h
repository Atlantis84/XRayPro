#ifndef GDATAFACTORY_H
#define GDATAFACTORY_H
#include "mainwindow.h"
#include "topwidget.h"
#include "centralwgt.h"
#include "aboutwgt.h"
#include "visiontemplatewgt.h"
#include "zoommoveobj.h"
#include "templateconfigwgt.h"
#include "pixmapwgt.h"
#include "processinfowgt.h"
#include "processpixmapwgt.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include "Detector.h"
#include "elecmanualwgt.h"
#include "datacommserial.h"
#include "udpservice.h"
#include "paraconfigwgt.h"
#include "welcomedlg.h"
#include "monitorthread.h"
#include "dynamiccirclewgt.h"
#include "equipinfowgt.h"
#include "productmanagewgt.h"
#include "productstyledlg.h"
#include <QMutex>
#include "CameraInterface.h"
#include "BarCodeDecoding.h"
#include "dmdbaccessinterface.h"
#include <math.h>
#include <QSqlQuery>
#include "testwindow.h"
#include <QTime>
#include "waitcountwgt.h"
#include <QThread>
#include <QNetworkAccessManager>
#include "imessagebox.h"
#include <QDir>
using namespace std;
using namespace cv;
#define READ_PLC  0x01 //function code of read plc
#define WRITE_PLC 0x02 //function code of write plc

#define ADDRESS_D200 0x00C8
#define ADDRESS_D202 0x00CA

#define ADDRESS_W201 0x00C9
#define ADDRESS_W201_00 0x00
#define ADDRESS_W201_01 0x01

#define ADDRESS_W203 0x00CB
#define ADDRESS_W203_00 0x00
#define ADDRESS_W203_01 0x01
#define ADDRESS_W203_02 0x02
#define ADDRESS_W203_03 0x03

#define ADDRESS_W60 0x003C
#define ADDRESS_W60_00 0x00
#define ADDRESS_W60_01 0x01
#define ADDRESS_W60_02 0x02
#define ADDRESS_W60_03 0x03
#define ADDRESS_W60_04 0x04
#define ADDRESS_W60_05 0x05
#define ADDRESS_W60_06 0x06
#define ADDRESS_W60_07 0x07
#define ADDRESS_W60_08 0x08
#define ADDRESS_W60_09 0x09
#define ADDRESS_W60_0A 0x0A
#define ADDRESS_W60_0B 0x0B

#define ADDRESS_W209 0xD1
#define ADDRESS_W210 0xD2

enum System_Status{
    Init_Status = 0,
    Run_Status = 1,
    Manual_Status = 2,
    Calibrate_Status = 3
};

enum Current_Product_Style{
    Product_01005 = 0,
    Product_04002 = 1,
    Product_06003 = 2,
    Product_08005 = 3
};

class NormalCountThread :public QThread
{
    Q_OBJECT
public:
    explicit NormalCountThread(QWidget *parent = nullptr){}

private:
    void run();
signals:
    void signal_normal_count_over(int amount,double counttime);
};

class GDataFactory:public QObject
{
    Q_OBJECT
public:
    GDataFactory();
    void scan_camera_mat_to_pixmap(const Mat& cvImage);
    QMap<QString,QString> get_product_info();

    void set_current_product_style(QString style)
    {
        m_pCurrentProductStyle = style;
    }
    QString get_current_product_style()
    {
        return m_pCurrentProductStyle;
    }
private:
    NormalCountThread* m_pNormalCountThread;
    QFile m_pConfigFile;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;
    QString m_pConfigFileName;
    QMap<QString,QString> ConfigInfo;
    DataCommSerial* pComm;
    System_Status m_pSystemStatus;
//    Current_Product_Style m_pProductStyle;
    QMutex m_mutex;

    cv::Mat m_pCurrentImage;
    cv::Mat m_pCurrentTemplateImage;

    QString m_pCurrentVoltage;
    QString m_pCurrentCurrent;
    QList<QString> m_pVoltageList;
    QList<QString> m_pCurrentList;

    QString m_pCurrentProductStyle;
    QString m_pCurrentSecondSectionCode;
    QString m_pCurrentFullSN;
    void get_step_and_threshold_run(float &step, float &threshold);



public:
    void submit_msg_to_mes(QString currentSN,QString productQuantity);

    void set_current_full_sn(QString strSN)
    {
        m_pCurrentFullSN = strSN;
    }

    QString get_current_full_sn()

    {
        return m_pCurrentFullSN;
    }

    void set_current_second_section_code(QString strCode)
    {
        m_pCurrentSecondSectionCode = strCode;
    }
    void raw_to_mat(unsigned short* pBuffer, const int nWidth, const int nHeight);
    QImage mat_to_qimage(const Mat& cvImage);
    QPixmap mat_to_pixmap(const Mat& cvImage);

    void set_sender_power(QString strStyle);

    QList<QString> get_voltage_list()
    {
        return m_pVoltageList;
    }

    QList<QString> get_current_list()
    {
        return m_pCurrentList;
    }

    QList<QString> get_product_style_list();

    void set_sender_voltage(QString strV)
    {
        m_pCurrentVoltage = strV;
    }

    QString get_sender_voltage()
    {
        return m_pCurrentVoltage;
    }

    void set_sender_current(QString strC)
    {
        m_pCurrentCurrent = strC;
    }

    QString get_sender_current()
    {
        return m_pCurrentCurrent;
    }

    void count_product();

    void pre1(Mat & src,Point center=Point(1563,1563),int r=120);

    void pre2(Mat & src)
    {
        Mat gray, binary;
        cvtColor(src, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 200, 255, THRESH_BINARY_INV);
        Mat kernel = Mat::ones(Size(15, 15), CV_8UC1);
        morphologyEx(binary, binary, MORPH_OPEN, kernel);
        Mat kerne2 = Mat::ones(Size(55, 55), CV_8UC1);
        dilate(binary, binary, kerne2);

        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;
        findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++)
        {
            float are = cv::contourArea(contours[i]);
            if (are > 50000)
            {
                drawContours(src, contours, i, Scalar(255, 255, 255), -1);

            }
        }
    }

    void pre3(Mat & src)
    {
        Mat gray, binary;
        cvtColor(src, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 200, 255, THRESH_BINARY_INV);
        Mat kernel = Mat::ones(Size(55, 55), CV_8UC1);
        morphologyEx(binary, binary, MORPH_CLOSE, kernel);
        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;
        findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        float max_are = 0;
        int index = 0;
        for (int i = 0; i < contours.size(); i++)
        {
            float are = contourArea(contours[i]);
            if (are > max_are)
            {
                max_are = are;
                index = i;
            }
        }

        Rect rect=boundingRect(contours[index]);
        rect.x -= 30;
        rect.y -= 30;
        rect.width += 60;
        rect.height += 60;

        src = src(rect);
    }

    void pre_big(Mat& src)
    {
        Mat gray, binary;
        cvtColor(src, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 200, 255, THRESH_BINARY_INV);

        Mat kernel = Mat::ones(Size(30, 30), CV_8UC1);
        morphologyEx(binary, binary, MORPH_CLOSE, kernel);



        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;
        findContours(binary, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        int index = -1;
        float max_area = 0;
        for (int i = 0; i < contours.size();i++)
        {
            float area = contourArea(contours[i]);
            if (area > max_area)
            {
                index = i;
                max_area = area;
            }
        }
        Rect rect = boundingRect(contours[index]);
        rect.x -= 20;
        rect.y -= 20;
        rect.width += 20;
        rect.height += 20;

        src = src(rect);
    }

    cv::Mat get_current_image_mat()
    {
        return m_pCurrentImage;
    }

    cv::Mat get_current_template_image_mat()
    {
        return m_pCurrentTemplateImage;
    }

    void set_current_template_image_mat(cv::Mat img)
    {
        m_pCurrentTemplateImage = img;
    }

    void set_current_bar_code(QString codeFromCamera)
    {
        emit signal_notify_bar_code(codeFromCamera);
    }

    QString get_product_style(QString strSN);

    void set_system_status(System_Status status)
    {
        QMutexLocker lock(&m_mutex);
        this->m_pSystemStatus = status;
    }

    System_Status get_system_status(){
        QMutexLocker lock(&m_mutex);
        System_Status aa = m_pSystemStatus;
        return aa;
    }

    void release_receiver_res()
    {
        return;
        if(get_detector_obj())
        {
            get_detector_obj()->Destroy();
            get_detector_obj()->FreeIRayLibrary();
            delete m_pDetectorObj;
            m_pDetectorObj = NULL;
        }
    }

    void connections_initialization();
    int initialize_xray();
    void get_xray_image();
    void load_json_config(char *filename);
    QString get_config_para(const QString keyname);
    void save_config_file();
    void set_config_para(const QString keyname, const QString keyvalue);

    quint8 get_last_ip(const int id);
    quint8 process_ip_string(const QString ipstr);

    void read_serial_number_xray(int functioncode);
    QString bytes_to_str(QByteArray data);

    QStringList find_files(const QString& startDir,QStringList filters,bool _isParent){
        //m_filters << "*.pdb" << "*.exp"  << "*.db" << "*.log";
        QStringList names;
        QDir dir(startDir);
        foreach(QString file, dir.entryList(filters, QDir::Files))
            names += startDir + '/' + file;
        if (_isParent) return names;
        foreach(QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
            names += find_files(startDir + '/' + subdir, filters, _isParent);
        return names;
    }

    void delete_files(QString folder)
    {
        QStringList names;
        QStringList m_filters;
        m_filters<<"*.jpg";
        names = find_files(folder, m_filters, true);
        for (int i=0;i< names.size();i++)
        {
            QString strPath = names.at(i);
            if (strPath.isEmpty() || !QDir().exists(strPath))
                return;

            QFileInfo FileInfo(strPath);
            if (FileInfo.isFile())
                QFile::remove(strPath);
        }
    }

    static GDataFactory* get_factory()
    {
        if(m_pDataFactory == nullptr)
            m_pDataFactory = new GDataFactory();
        return m_pDataFactory;
    }

    static TopWidget* get_top_wgt()
    {
        if(m_pTopWgt == nullptr)
            m_pTopWgt = new TopWidget();
        return m_pTopWgt;
    }

    static MainWindow* get_main_window()
    {
        if(m_pMainWindow == nullptr)
            m_pMainWindow = new MainWindow();
        return m_pMainWindow;
    }

    static CentralWgt* get_central_wgt()
    {
        if(m_pCentralWgt == nullptr)
            m_pCentralWgt = new CentralWgt();
        return m_pCentralWgt;
    }

    static AboutWgt* get_about_wgt()
    {
        if(m_pAboutWgt == nullptr)
            m_pAboutWgt = new AboutWgt();
        return m_pAboutWgt;
    }

    static VisionTemplateWgt* get_vision_template_wgt()
    {
        if(m_pVisionTemplateWgt == nullptr)
            m_pVisionTemplateWgt = new VisionTemplateWgt();
        return m_pVisionTemplateWgt;
    }

    static ZoomMoveObj* get_zoom_obj()
    {
        if(m_pZoomMoveObj == nullptr)
            m_pZoomMoveObj = new ZoomMoveObj();
        return m_pZoomMoveObj;
    }

    static TemplateConfigWgt* get_template_config_wgt()
    {
        if(m_pTemplateConfigWgt == nullptr)
            m_pTemplateConfigWgt = new TemplateConfigWgt();
        return m_pTemplateConfigWgt;
    }

    static PixmapWgt* get_pixmap_wgt()
    {
        if(m_pPixmapWgt == nullptr)
            m_pPixmapWgt = new PixmapWgt();
        return m_pPixmapWgt;
    }

    static ProcessInfoWgt* get_process_info_wgt()
    {
        if(m_pProcessInfoWgt == nullptr)
            m_pProcessInfoWgt = new ProcessInfoWgt();
        return m_pProcessInfoWgt;
    }

    static ProcessPixmapWgt* get_process_pixmap_wgt()
    {
        if(m_pProcessPixmapWgt == nullptr)
            m_pProcessPixmapWgt = new ProcessPixmapWgt();
        return m_pProcessPixmapWgt;
    }

    static CDetector* get_detector_obj()
    {
        if(m_pDetectorObj == nullptr)
            m_pDetectorObj = new CDetector();
        return m_pDetectorObj;
    }

    static ElecManualWgt* get_elec_manual_wgt()
    {
        if(m_pElecManualWgt == nullptr)
            m_pElecManualWgt = new ElecManualWgt();
        return m_pElecManualWgt;
    }

    static void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
        const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);

    static UdpService* get_udp_service()
    {
        if(m_pUdpService == nullptr)
            m_pUdpService = new UdpService();
        return m_pUdpService;
    }

    static ParaConfigWgt* get_para_config_wgt()
    {
        if(m_pParaConfigWgt == nullptr)
            m_pParaConfigWgt = new ParaConfigWgt;
        return m_pParaConfigWgt;
    }

    static WelcomeDlg* get_welcome_dlg()
    {
        if(m_pWelcomeDlg == nullptr)
            m_pWelcomeDlg = new WelcomeDlg();
        return m_pWelcomeDlg;
    }

    static MonitorThread* get_monitor_thread()
    {
        if(m_pMonitorThread == nullptr)
            m_pMonitorThread = new MonitorThread();
        return m_pMonitorThread;
    }

    static DynamicCircleWgt* get_dynamic_circle_wgt()
    {
        if(m_pDynamicCircleWgt == nullptr)
            m_pDynamicCircleWgt = new DynamicCircleWgt();
        return m_pDynamicCircleWgt;
    }

    static EquipInfoWgt* get_equip_info_wgt()
    {
        if(m_pEquipInfoWgt == nullptr)
            m_pEquipInfoWgt = new EquipInfoWgt();
        return m_pEquipInfoWgt;
    }

    static ProductStyleDlg* get_product_style_dlg()
    {
        if(m_pProductStyleDlg == nullptr)
            m_pProductStyleDlg = new ProductStyleDlg();
        return m_pProductStyleDlg;
    }

    static CameraInterface* get_camera_interface()
    {
        if(m_pCameraInterface == nullptr)
            m_pCameraInterface = new CameraInterface();

        return m_pCameraInterface;
    }

    static BarCodeDecoding* get_bar_code_decoding()
    {
        if(m_pBarCodeDecoding == nullptr)
            m_pBarCodeDecoding = new BarCodeDecoding();
        return m_pBarCodeDecoding;
    }

    static DMDBAccessInterface* get_pgsql()
    {
        if(m_pDMDBAccess == nullptr)
            m_pDMDBAccess = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccess;
    }

    static TestWindow* get_test_window()
    {
        if(m_pTestWindow == nullptr)
            m_pTestWindow = new TestWindow();
        return m_pTestWindow;
    }

    static QTime* get_time_step_counter()
    {
        if(m_pTimeStepCounter == nullptr)
            m_pTimeStepCounter = new QTime();
        return m_pTimeStepCounter;
    }

    static ProductManageWgt* get_product_manage_wgt()
    {
        if(m_pProductManageWgt == nullptr)
            m_pProductManageWgt = new ProductManageWgt();
        return m_pProductManageWgt;
    }

    static WaitCountWgt* get_wait_count_wgt()
    {
        if(m_pWaitCountWgt == nullptr)
            m_pWaitCountWgt = new WaitCountWgt();
        return m_pWaitCountWgt;
    }

    static QNetworkAccessManager* get_net_work_access_manager()
    {
        if(m_pAccessManager == nullptr)
            m_pAccessManager = new QNetworkAccessManager();
        return m_pAccessManager;
    }
private:
    static TopWidget* m_pTopWgt;
    static MainWindow* m_pMainWindow;
    static CentralWgt* m_pCentralWgt;
    static AboutWgt* m_pAboutWgt;
    static VisionTemplateWgt* m_pVisionTemplateWgt;
    static ZoomMoveObj* m_pZoomMoveObj;
    static GDataFactory* m_pDataFactory;
    static TemplateConfigWgt* m_pTemplateConfigWgt;
    static PixmapWgt* m_pPixmapWgt;
    static ProcessInfoWgt* m_pProcessInfoWgt;
    static ProcessPixmapWgt* m_pProcessPixmapWgt;
    static CDetector* m_pDetectorObj;
    static ElecManualWgt* m_pElecManualWgt;
    static UdpService* m_pUdpService;
    static ParaConfigWgt* m_pParaConfigWgt;
    static WelcomeDlg* m_pWelcomeDlg;
    static MonitorThread* m_pMonitorThread;
    static DynamicCircleWgt* m_pDynamicCircleWgt;
    static EquipInfoWgt* m_pEquipInfoWgt;
    static ProductStyleDlg* m_pProductStyleDlg;
    static CameraInterface* m_pCameraInterface;
    static BarCodeDecoding* m_pBarCodeDecoding;
    static DMDBAccessInterface* m_pDMDBAccess;
    static TestWindow* m_pTestWindow;
    static QTime* m_pTimeStepCounter;
    static ProductManageWgt* m_pProductManageWgt;
    static WaitCountWgt* m_pWaitCountWgt;
    static QNetworkAccessManager *m_pAccessManager;
signals:
    void signal_spread_pixmap_to_ui(const QPixmap pm);
    void signal_serial_data(QByteArray data);
    void signal_notify_count_result(int countR,double aTime);
    void signal_notify_bar_code(QString pStyle);
    void signal_notify_time_step(QString step);
    void signal_start_to_count();
    void signal_send_current_threshold(QString tr);
public slots:
    void slot_start_to_count();
    void slot_rev_serial_number(const QByteArray& data);
    void slot_normal_count_over(int amount,double counttime);
};

#endif // GDATAFACTORY_H
