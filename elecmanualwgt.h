#ifndef ELECMANUALWGT_H
#define ELECMANUALWGT_H

#include <QWidget>
#include "circlewidget.h"
class QPushButton;

class ElecManualWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ElecManualWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    int m_pTimerID;

    CircleWidget* circleFrev;
    CircleWidget* circleWatchDog;
    CircleWidget* circleClr;
    CircleWidget* circleEnable;
    CircleWidget* circleOn;
    CircleWidget* circleVp;
    CircleWidget* circleCp;
    CircleWidget* circleUnable;
    QList<CircleWidget*> lstCircleWgtComm;

    CircleWidget* circle1;
    CircleWidget* circle2;
    CircleWidget* circle3;
    CircleWidget* circle4;
    CircleWidget* circle5;
    CircleWidget* circle6;
    CircleWidget* circle7;
    CircleWidget* circle8;
    CircleWidget* circle9;
    CircleWidget* circle10;
    CircleWidget* circle11;
    CircleWidget* circle12;
    CircleWidget* circle13;
    CircleWidget* circle14;
    CircleWidget* circle15;
    CircleWidget* circle16;
    CircleWidget* circle17;
    CircleWidget* circle18;
    CircleWidget* circle19;
    CircleWidget* circle20;
    CircleWidget* circle21;
    CircleWidget* circle22;
    CircleWidget* circle23;
    CircleWidget* circle24;
    QList<CircleWidget*> lstCircleWgt;

    QPushButton* btnFrev;
    QPushButton* btnWatchDog;
    QPushButton* btnClr;
    QPushButton* btnEnable;
    QPushButton* btnOn;
    QPushButton* btnVp;
    QPushButton* btnCp;
    QPushButton* btnUnable;


signals:
    void signal_process_serial_info(int equipID,QString info);

public slots:
    void slot_btn_click(int btnID);
    void slot_process_serial_data(QByteArray data);
    void slot_process_udp_data(QByteArray data);
};

#endif // ELECMANUALWGT_H
