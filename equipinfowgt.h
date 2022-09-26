#ifndef EQUIPINFOWGT_H
#define EQUIPINFOWGT_H

#include <QWidget>
#include <QLabel>
class EquipInfoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit EquipInfoWgt(QWidget *parent = nullptr);
    void set_text(int eqID,QString info);
private:
    void paintEvent(QPaintEvent *event);

    int m_pCurrentEquipID;//1 is serial port; 2 is receiver;
    QString m_pCurrentInfo;
    QLabel* m_pLabelInfo;

signals:

public slots:
    void slot_process_info(int equipID,QString info);
};

#endif // EQUIPINFOWGT_H
