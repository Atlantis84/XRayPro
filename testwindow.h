#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <QLineEdit>
class TestWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = nullptr);

    QLineEdit* leNumber;
    QLineEdit* leLog;
signals:

public slots:
    void slot_take_photo();
};

#endif // TESTWINDOW_H
