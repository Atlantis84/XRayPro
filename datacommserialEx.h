#ifndef DataCommSerialEx_H
#define DataCommSerialEx_H

#include <QThread>
#include <QSerialPort>
struct tagSerialPortInfoEx
{
    QString                     portName;
    qint32                      baudRate;
    QSerialPort::Directions     directions;
    QSerialPort::DataBits       dataBits;
    QSerialPort::FlowControl    flowControl;
    QSerialPort::Parity         parity;
    QSerialPort::StopBits       stopBits;

    tagSerialPortInfoEx()
    {
        directions = QSerialPort::AllDirections;
        dataBits = QSerialPort::Data8;
        flowControl = QSerialPort::NoFlowControl;
        parity = QSerialPort::NoParity;
        stopBits = QSerialPort::OneStop;
    }

    tagSerialPortInfoEx(QString name, qint32 rate):
        tagSerialPortInfoEx()
    {
        portName = name;
        baudRate = rate;
    }
};

class IResponseValidateEx
{
public:
    virtual bool validate(const QByteArray& response) = 0;
    virtual ~IResponseValidateEx(){}
};

class DataCommSerialEx : public QThread
{
    Q_OBJECT
public:

    enum SerialWorkModeEx
    {
        serial_send_time_to_live,
        serial_send_request_only,
        serial_send_request_recv_response,
        serial_send_request_check_response,
        serial_recv_response_only,
    };

    explicit DataCommSerialEx(QObject *parent = nullptr);

    // ��ʼ��������Ϣ
    void init(const tagSerialPortInfoEx& info);

    // ����ttl�ź�
    void send_time_to_live(int interval_time_ms, int timeout_ms);

    // ����������
    void sendRequest_only(const QList<QByteArray>& datas, int timeout_ms);

    // �����ҽ���Ӧ��
    void sendRequest_and_recvResponse(const QList<QByteArray>& datas, int timeout_ms);

    // �����Ҽ��Ӧ��
    void sendRequest_and_checkResponse(const QList<QByteArray>& datas, int timeout_ms, IResponseValidateEx* pValidate);

    // ����������
    void recvResponse_only();

    void run();
    void closeSerialPort()
    {
        serialport.close();
    }

signals:
    // Ӧ�������ź�
    void sigRecvResponse(const QByteArray& data);

    // У��Ӧ�������ź�
    void sigCheckResponse(bool valid, const QByteArray& data);

    // �쳣��ʾ�ź�
    void sigCatchException(const QString& info);

public slots:

private:
    QList<QByteArray>       m_request_datas;
    int                     m_ttl_interval_ms;
    int                     m_wait_timeout;
    tagSerialPortInfoEx       m_serialport_info;
    SerialWorkModeEx          m_serial_work_mode;
    IResponseValidateEx*      m_pResponseValidate;
    QSerialPort serialport;
};


#endif // DataCommSerialEx_H
