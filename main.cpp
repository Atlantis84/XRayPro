#include "mainwindow.h"
#include <QApplication>
#include "gdatafactory.h"
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include "QsLog.h"
#include "QsLogDest.h"
#include <QDesktopWidget>
#include "split.h"
#include "TemplateBasedMethod.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
using namespace QsLogging;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString qssFile = ":/icon/darkgray.qss";
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    if(GDataFactory::get_pgsql()->ConnectDataBase())
        QLOG_INFO()<<"connect to db success!";
    else {
        QLOG_FATAL()<<"connect to db failed!";
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"链接数据库失败,程序退出!");
        return -1;
    }


//    if(GDataFactory::get_welcome_dlg()->exec() == QDialog::Accepted)
//        ;
//    else
//        QApplication::exit(1);

    Logger& logger = Logger::instance();
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath("log.txt"));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(512*1024), MaxOldLogCount(5)));
    logger.addDestination(fileDestination);

    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);

    DestinationPtr controlDestination(DestinationFactory::MakeFunctorDestination(GDataFactory::get_process_info_wgt(),SLOT(slot_rev_logs(const QString))));
    logger.addDestination(controlDestination);
    QLOG_INFO()<<u8"QsLog init SUCCESS";

    int okValue = GDataFactory::get_camera_interface()->OpenCamera();
    if(okValue == 0)
        QLOG_INFO()<<"open camera success";
    else {
        QLOG_WARN()<<"open camera failed";
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"打开相机失败,程序退出!");
        return -1;
    }


    QString binDir=QApplication::applicationDirPath();
//    GDataFactory::get_factory()->delete_files(binDir);
    QString fileName("config.json");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using json config file"<<fileName;
            GDataFactory::get_factory()->load_json_config(QByteArray(fileName.toLatin1()).data());
        }
        else
        {
            QLOG_WARN()<<"Json config file not exist"<<fileName;
        }
    }

    QString logLevel = GDataFactory::get_factory()->get_config_para("LOG_LEVEL");
    if(logLevel == "0")
        logger.setLoggingLevel(QsLogging::TraceLevel);
    else if(logLevel == "1")
        logger.setLoggingLevel(QsLogging::DebugLevel);
    else if(logLevel == "2")
        logger.setLoggingLevel(QsLogging::InfoLevel);
    else if(logLevel == "3")
        logger.setLoggingLevel(QsLogging::WarnLevel);
    else if(logLevel == "4")
        logger.setLoggingLevel(QsLogging::ErrorLevel);
    else if(logLevel == "5")
        logger.setLoggingLevel(QsLogging::FatalLevel);
    else
        logger.setLoggingLevel(QsLogging::OffLevel);

    GDataFactory::get_factory()->connections_initialization();

    GDataFactory::get_factory()->set_system_status(Init_Status);
    GDataFactory::get_monitor_thread()->start();
    GDataFactory::get_welcome_dlg()->exec();

    GDataFactory::get_factory()->read_product_code_number();
    QDesktopWidget *desktop = QApplication::desktop();
    GDataFactory::get_top_wgt()->move((desktop->width()-GDataFactory::get_top_wgt()->width())/2,
                                      (desktop->height()-GDataFactory::get_top_wgt()->height())/2);
    GDataFactory::get_top_wgt()->show();

    return a.exec();
}
