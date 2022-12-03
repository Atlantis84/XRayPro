#ifndef PROCESSINFOWGT_H
#define PROCESSINFOWGT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QTextEdit>
#include "leftwidget.h"
#include <QComboBox>
#include <QPushButton>
class ProcessInfoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessInfoWgt(QWidget *parent = nullptr);

private:
    QTextEdit* m_pLogEdit;
    QLineEdit* m_pProductAmount;
    QLineEdit* m_pCountTime;
    QLineEdit* m_pProductStyle;
    QLineEdit* m_pCountStep;
    QLineEdit* m_pCurrentThreshold;
    QLineEdit* m_pCurrentStyle;

    QComboBox* m_pCmbProductStyle;
    QComboBox* m_pCmbThreshold;
    QPushButton* btnUpdate;

    LeftWidget* m_pLeftWgt;

signals:

public slots:
    void slot_rev_logs(const QString log);
    void slot_rev_count_result(int countR,double aTime);
    void slot_rev_bar_code(QString pStyle);
    void slot_rev_count_step(QString step);
    void slot_update_threshold();
    void slot_rev_button_control_sign(bool sign);
    void slot_rev_current_threshold(QString tr);
};

#endif // PROCESSINFOWGT_H
