#ifndef WELCOMEDLG_H
#define WELCOMEDLG_H

#include <QDialog>
#include "equipinfowgt.h"
#include <QLineEdit>
class WelcomeDlg : public QDialog
{
    Q_OBJECT
public:
    WelcomeDlg();
public:
    void set_text(int eqID,QString info);
private:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    QLineEdit* m_pLineAccount;
public slots:
    void slot_ok();
    void slot_cancel();
};

#endif // WELCOMEDLG_H
