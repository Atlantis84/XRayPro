#ifndef PROCESSINFOWGT_H
#define PROCESSINFOWGT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QTextEdit>
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

signals:

public slots:
    void slot_rev_logs(const QString log);
    void slot_rev_count_result(int countR,double aTime);
    void slot_rev_bar_code(QString pStyle);
    void slot_rev_count_step(QString step);
};

#endif // PROCESSINFOWGT_H
