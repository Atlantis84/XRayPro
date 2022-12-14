#ifndef DATACOMMSERIAL_H
#define DATACOMMSERIAL_H

#include <QThread>
#include <QSerialPort>

struct tagSerialPortInfo
{
    QString                     portName;
    qint32                      baudRate;
    QSerialPort::Directions     directions;
    QSerialPort::DataBits       dataBits;
    QSerialPort::FlowControl    flowControl;
    QSerialPort::Parity         parity;
    QSerialPort::StopBits       stopBits;

    tagSerialPortInfo()
    {
        directions = QSerialPort::AllDirections;
        dataBits = QSerialPort::Data8;
        flowControl = QSerialPort::NoFlowControl;
        parity = QSerialPort::NoParity;
        stopBits = QSerialPort::OneStop;
    }

    tagSerialPortInfo(QString name, qint32 rate):
        tagSerialPortInfo()
    {
        portName = name;
        baudRate = rate;
    }
};

enum SerialWorkMode
{
    serial_send_time_to_live,
    serial_send_request_only,
    serial_send_request_recv_response,
    serial_send_request_check_response,
    serial_recv_response_only,
};

class IResponseValidate
{
public:
    virtual bool validate(const QByteArray& response) = 0;
    virtual ~IResponseValidate(){}
};

class DataCommSerial : public QObject
{
    Q_OBJECT
public:
    explicit DataCommSerial(QObject *parent = nullptr);

    // 初始化串口信息
    void init(const tagSerialPortInfo& info);

    // 发送ttl信号
    void send_time_to_live(int interval_time_ms, int timeout_ms);

    // 仅发送数据
    void sendRequest_only(const QList<QByteArray>& datas, int timeout_ms);

    // 发送且接收应答
    void sendRequest_and_recvResponse(const QList<QByteArray>& datas, int timeout_ms);

    // 发送且检查应答
    void sendRequest_and_checkResponse(const QList<QByteArray>& datas, int timeout_ms, IResponseValidate* pValidate);

    // 仅接受数据
    void recvResponse_only();

    void run();
    void closeSerialPort()
    {
        serialport.close();
    }

signals:
    // 应答数据信号
    void sigRecvResponse(const QByteArray& data);

    // 校验应答数据信号
    void sigCheckResponse(bool valid, const QByteArray& data);

    // 异常提示信号
    void sigCatchException(const QString& info);

public slots:

private:
    QList<QByteArray>       m_request_datas;
    int                     m_ttl_interval_ms;
    int                     m_wait_timeout;
    tagSerialPortInfo       m_serialport_info;
    SerialWorkMode          m_serial_work_mode;
    IResponseValidate*      m_pResponseValidate;
    QSerialPort serialport;
};

#endif // DATACOMMSERIAL_H
