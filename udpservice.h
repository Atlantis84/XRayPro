#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QMutex>
class UdpService : public QObject
{
    Q_OBJECT
public:
    explicit UdpService(QObject *parent = nullptr);

    void release_res();
    void send_message_to_plc(const uchar function_code, const short data_address,const uchar bit_address,const short data_length/*length is word length*/,const QByteArray data_array);
    void send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
//    void send_message_to_plc(const uchar function_code, const short data_address,char* section_sign/*0x01 D-section-word 0x02 W-section-word*/,const short data_length/*length is word length*/,const QByteArray data_array);
private:
    QUdpSocket *m_pUdpSocket;
    QByteArray make_plc_message(const uchar function_code, const short data_address,const uchar bit_address,const short data_length/*length is word length*/,const QByteArray data_array);
    QByteArray make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
//    QByteArray make_plc_message(const uchar function_code, const short data_address,char* section_sign/*0x01 D-section 0x02 W-section*/,const short data_length/*length is word length*/,const QByteArray data_array);
    QMutex m_mutex;

signals:
    void signal_proc_plc_msg(const QByteArray data_array);
    void signal_notify_app_by_plc(const QByteArray data);
    void signal_notify_app_by_manual(const QByteArray data);

public slots:
    void slot_receive_message();
    void slot_proc_plc_msg(const QByteArray data_array);
};

#endif // UDPSERVICE_H
