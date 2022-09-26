#include "titlewidget.h"
#include <QPainter>
#include <QColor>
#include <QPen>

TitleWidget::TitleWidget(QWidget *parent) : QWidget(parent)
{

}

void TitleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(QColor(255,255,255),1));
    painter.drawRect(this->rect());
}
