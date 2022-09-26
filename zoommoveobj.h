#ifndef ZOOMMOVEOBJ_H
#define ZOOMMOVEOBJ_H
#include <QGraphicsView>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include "opencv2/opencv.hpp"
class ZoomMoveObj : public QGraphicsView
{
    Q_OBJECT
public:
    ZoomMoveObj();

    void set_product_style(QString strStyle)
    {
        m_product_style = strStyle;
    }
private:
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    int scale_m;
    bool isMousePressed;
    QPointF centerAnchor;
    QPoint posAnchor;
    QPointF startPtf;
    QPointF endPtf;
    QGraphicsRectItem *rectItem;
    QGraphicsScene *m_scene;
    QPixmap pm;
    QGraphicsPixmapItem* m_imageItem;

    QString m_product_style;

signals:
    void signal_transfer_pixmap(QPixmap p);
public slots:
    void slot_take_image();
    void slot_rev_pixmap(QPixmap p);
};

#endif // ZOOMMOVEOBJ_H
