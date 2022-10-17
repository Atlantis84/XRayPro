#include "monitorthread.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QProcess>
#include <QTextCodec>
MonitorThread::MonitorThread()
{

}
bool sendsign = false;
void MonitorThread::run()
{
    while (1) {

        while(1)
        {
            emit signal_send_receiver_info(2,u8"唤醒PLC中...");
            QProcess pingProcess;
            QString strArg = "ping " + GDataFactory::get_factory()->get_config_para("PLC_IP") + " -n 1 -i 2";
            pingProcess.start(strArg,QIODevice::ReadOnly);
            pingProcess.waitForFinished(-1);

            QTextCodec* tc = QTextCodec::codecForName("GBK");
            QString p_stdout = tc->toUnicode(pingProcess.readAllStandardOutput());

            if(p_stdout.contains("TTL="))
            {
               break;
            }
            QThread::msleep(500);
        }

        if(GDataFactory::get_factory()->get_system_status() == Manual_Status)
        {
            emit signal_send_receiver_info(2,u8"接收源初始化中...");
            if(!GDataFactory::get_factory()->initialize_xray())
            {
                emit signal_send_receiver_info(2,u8"接收源初始化成功!");
                QThread::msleep(2000);
                GDataFactory::get_welcome_dlg()->close();
                break;
            }
            else {
                emit signal_send_receiver_info(2,u8"接收源初始化失败!");
                QThread::msleep(2000);
                GDataFactory::get_welcome_dlg()->close();
                break;
            }

        }
        if(!sendsign)
        {
            QLOG_INFO()<<"Init Sender";
            sendsign = true;
            GDataFactory::get_factory()->read_serial_number_xray(0);//FREV
        }
        QThread::msleep(50);
    }
}
