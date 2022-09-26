#ifndef TEMPLATECONFIGWGT_H
#define TEMPLATECONFIGWGT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QThread>
class CountThread: public QThread
{
    Q_OBJECT
public:
    explicit CountThread(QWidget *parent = nullptr);

private:
    void run();
signals:
    void signal_count_over(int amount);
};

class TemplateConfigWgt : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateConfigWgt(QWidget *parent = nullptr);
    ~TemplateConfigWgt();
private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void update_power_para();
    void get_step_and_threshold(float& step, float& threshold);

    QLineEdit* le_product_num;
    QLineEdit* le_product_style;
    QLineEdit* le_product_amount;
    QComboBox* cmb_product_plate_size;
    QComboBox* cmb_product_style;

    QComboBox* cmb_sender_current;
    QComboBox* cmb_sender_voltage;

    QComboBox* cmb_vision_step;
    QComboBox* cmb_vision_threshold;

    CountThread* m_pCountThread;
signals:
    void signal_notify_take_picture();

public slots:
    void slot_receiver_take_image();
    void slot_generate_template();
    void slot_count_test();
    void slot_set_sender_para();
    void slot_count_over(int amount);
};

#endif // TEMPLATECONFIGWGT_H
