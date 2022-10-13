#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void set_count_sign(bool sign)
    {
        m_pNormalTakePictureSign = sign;
    }
    ~MainWindow();
private:
    void createStatusBar();
    void createActions();
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

    int m_pTimerID;
    int m_pTimerMonitorID;
    int m_pTimerTakePictureOverID;
    bool m_pNormalTakePictureSign;
    int m_pTimeOut;

    QByteArray m_pData;

    QMenu *viewMenu;

    QAction *startRunAct;
    QAction *elecManualAct;
    QAction *visionTemplateAct;
    QAction *portSetAct;
    QAction *productManagementAct;

public slots:
    void about();
    void slot_start_run();
    void slot_create_vision_template();
    void slot_elec_manual_control();
    void slot_para_set();
    void slot_proc_plc_notify(QByteArray data);
    void slot_product_management();
signals:
    void signal_enable_disable_threshold_button(bool sign);
};

#endif // MAINWINDOW_H
