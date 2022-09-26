#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

signals:

public slots:
};

#endif // TOPWIDGET_H
