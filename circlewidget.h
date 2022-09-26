#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QWidget>

class CircleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CircleWidget(QWidget *parent = nullptr);
    void set_color(int colorsign);//0:green 1:red 2:blue
private:
    void paintEvent(QPaintEvent *event);
    int m_pColorSign;
public:

signals:

public slots:
};

#endif // CIRCLEWIDGET_H
