#include "monitorthread.h"
#include "gdatafactory.h"
#include "QsLog.h"
MonitorThread::MonitorThread()
{

}
bool sendsign = false;
void MonitorThread::run()
{
    while (1) {
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
