#ifndef WELCOMEDLG_H
#define WELCOMEDLG_H

#include <QDialog>
#include "equipinfowgt.h"
class WelcomeDlg : public QDialog
{
public:
    WelcomeDlg();
public:
    void set_text(int eqID,QString info);
private:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // WELCOMEDLG_H
