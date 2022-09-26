#include "udpservice.h"
#include "gdatafactory.h"
#include "QsLog.h"
UdpService::UdpService(QObject *parent) : QObject(parent)
{
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocket->bind(QHostAddress("192.168.0.189"),59999);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
}

void UdpService::release_res()
{
    disconnect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    disconnect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    if(m_pUdpSocket != nullptr)
        delete m_pUdpSocket;
    m_pUdpSocket = nullptr;
}

void UdpService::send_message_to_plc(const uchar function_code, const short data_address, const uchar bit_address,const short data_length, const QByteArray data_array)
{
    m_mutex.lock();
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,bit_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_mutex.unlock();
}

QByteArray UdpService::make_plc_message(const uchar function_code, const short data_address, const uchar bit_address,const short data_length, const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //起始码
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x31));//W section byte read and write
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(bit_address));
    int length = data_length;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

void UdpService::slot_receive_message()
{
    m_mutex.lock();
    if(m_pUdpSocket == nullptr)
        return;
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_plc_msg(datagram);
}

void UdpService::slot_proc_plc_msg(const QByteArray data_array)
{
    QLOG_INFO()<<"REV PLC MSG:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
    if(data_array.length() > 10)//check the header length
    {
        if((data_array[10] == 0x01) && (data_array[11] == 0x01))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
                QByteArray tmpArray;
                for(int i=14;i<data_array.length();i++)
                    tmpArray.append(data_array[i]);
                if(GDataFactory::get_factory()->get_system_status() == Run_Status)
                    emit signal_notify_app_by_plc(tmpArray);
                else if(GDataFactory::get_factory()->get_system_status() == Manual_Status)
                    emit signal_notify_app_by_manual(tmpArray);
            }
            else
            {
                QLOG_WARN()<<"PLC read back message is ABNORMAL!";
            }
        }
        else if((data_array[10] == 0x01) && (data_array[11] == 0x02))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
                ;
            }
            else
            {
                QLOG_WARN()<<"PLC write back message ABNORMAL!";
            }
        }
    }
    else
    {
        QLOG_WARN()<<"PLC message header lenght is WRONG!";
    }
}

void UdpService::send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    m_mutex.lock();
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_mutex.unlock();
}

QByteArray UdpService::make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //起始码
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    if((data_address == ADDRESS_D200) || (data_address == ADDRESS_D202))
        m_data.append(static_cast<char>(0x82));
    else
        m_data.append(static_cast<char>(0xB1));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

//void UdpService::send_message_to_plc(const uchar function_code, const short data_address,char* section_sign/*0x01 D-section 0x02 W-section*/,const short data_length/*length is word length*/,const QByteArray data_array)
//{
//    m_mutex.lock();
//    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
//    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
//    QByteArray btArray = make_plc_message(function_code,data_address,section_sign,data_length,data_array);
//    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
//    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
//    m_mutex.unlock();
//}

//QByteArray UdpService::make_plc_message(const uchar function_code, const short data_address,char* section_sign/*0x01 D-section-word 0x02 W-section-word*/,const short data_length/*length is word length*/,const QByteArray data_array)
//{
//    QByteArray m_data;
//    m_data.append(static_cast<char>(0x80));  //起始码
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x02));
//    m_data.append(static_cast<char>(0x00));  //PLC IP Address
//    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));  //self IP Address
//    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0xFF));  //SID
//    m_data.append(static_cast<char>(0x01));  //FINS

//    switch (function_code)
//    {
//    case 1://read plc
//        m_data.append(static_cast<char>(0x01));
//        break;
//    case 2://write plc
//        m_data.append(static_cast<char>(0x02));
//        break;
//    }

//    if(section_sign == 0x01)
//        m_data.append(static_cast<char>(0x82));
//    else if(section_sign == 0x02)
//        m_data.append(static_cast<char>(0xB1));
//    else
//        m_data.append(static_cast<char>(0x82));
//    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
//    m_data.append(static_cast<char>(0xFF&data_address));
//    m_data.append(static_cast<char>(0x00));
//    int length = data_length/2;//plc recognize by word
//    m_data.append(static_cast<char>(0xFF&(length>>8)));
//    m_data.append(static_cast<char>(0xFF&length));

//    if(function_code == 2)
//        m_data.append(data_array);

//    return m_data;
//}
