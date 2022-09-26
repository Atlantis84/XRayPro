#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // TITLEWIDGET_H
