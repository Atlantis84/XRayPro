#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QThread>
class MonitorThread : public QThread
{
    Q_OBJECT
public:
    MonitorThread();
protected:
    void run();
signals:
    void signal_send_receiver_info(int equipID,QString info);
public slots:

};

#endif // MONITORTHREAD_H
