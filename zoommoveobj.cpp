#include "zoommoveobj.h"
#include <QWheelEvent>
#include <QDebug>

#include "QsLog.h"
#include "gdatafactory.h"
using namespace QsLogging;
using namespace std;

cv::Mat image_to_mat(QPixmap p)
{
    QImage img = p.toImage();
    cv::Mat mat;
    mat = cv::Mat(img.height(),img.width(),CV_8UC3,img.bits(),img.bytesPerLine());

    return mat;
}

ZoomMoveObj::ZoomMoveObj()
{
    setDragMode(QGraphicsView::NoDrag);//(QGraphicsView::RubberBandDrag);//QGraphicsView::ScrollHandDrag
    scale_m = 1;//ͼ��ԭʼ����
    setStyleSheet("padding: 0px; border: 1px solid;border-color:rgba(0,0,0,100);");//�ޱ߿�
    setMouseTracking(true);//�������λ��
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//����ˮƽ��
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//��������
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    m_scene = new QGraphicsScene();
    m_imageItem = new QGraphicsPixmapItem();
    isMousePressed = false;
    rectItem = nullptr;

//    pm.load("f:\\1.tif");
//    m_imageItem->setPixmap(pm);
//    m_scene->addItem(m_imageItem);
//    this->setScene(m_scene);
//    this->update();
    this->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_product_style = "0402";
}

void ZoomMoveObj::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL)
    {
        //��סctrl�� ���ԷŴ���С
        if((event->delta() > 0)&&(scale_m >= 30))//���Ŵ�ԭʼͼ���50��
        {
            return;
        }
//        else if((event->delta() < 0)&&(scale_m <= 0.01))//ͼ����С������Ӧ��С֮��Ͳ�������С
//        {
//            return;//����ͼƬ��С��λ�ã�ʹ֮����Ӧ�ؼ����ڴ�С
//        }
        else
        {
            // ��ǰ��������;
            qreal scaleFactor = this->matrix().m11();
            scale_m = scaleFactor;

            int wheelDeltaValue = event->delta();
            // ���Ϲ������Ŵ�;
            if (wheelDeltaValue > 0)
            {
                this->scale(1.2, 1.2);
            }
            else
            {// ���¹�������С;
                this->scale(1.0 / 1.2, 1.0 / 1.2);
            }
            update();
        }
    }
}

void ZoomMoveObj::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)
    {
        if(this->scene() == nullptr)
        {
            qDebug() << "The scene is null";
            return;
        }
        // ��¼?�갴��ʱ����?������
        centerAnchor = mapToScene(event->pos()) - event->pos() + QPointF(width() / 2, height() / 2);
        // ��¼��ǰ?����view�е�λ�ã�?����mouseMove�¼��м���ƫ��
        // �˴�����view����ת����scene�����ԭ�����Ż����ܣ���move�Ĺ����л��?����
        posAnchor = event->pos();
        isMousePressed = true;
    }
    else if(event->modifiers() == Qt::SHIFT)
    {
        isMousePressed = true;
        startPtf = this->mapToScene(event->pos());
    }
    else if(event->button() == Qt::RightButton)
    {
        this->m_scene->removeItem(rectItem);
        this->viewport()->update();
    }
    QGraphicsView::mousePressEvent(event);
}

void ZoomMoveObj::mouseMoveEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)
    {
        QPointF offsetPos = event->pos() - posAnchor;
        if(isMousePressed){
            setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            centerOn(centerAnchor - offsetPos);
        }
    }
    else if(event->modifiers() == Qt::SHIFT)
    {
        if(isMousePressed)
        {
            m_scene->removeItem(rectItem);
            endPtf = this->mapToScene(event->pos());
            QRectF rf(startPtf.x(),startPtf.y(),endPtf.x()-startPtf.x(),endPtf.y()-startPtf.y());
            rectItem = new QGraphicsRectItem;
            rectItem->setPen(QPen(QColor(255,0,0),1));
            rectItem->setRect(rf);
            m_scene->addItem(rectItem);
            this->viewport()->update();
        }
    }
    else {
        QPointF Ptf = this->mapToScene(event->pos());
//        qDebug()<<"Ptf.x():"<<Ptf.x();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ZoomMoveObj::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (event->modifiers() == Qt::CTRL)
    {
        isMousePressed = false;
    }
    else if (event->modifiers() == Qt::SHIFT)
    {
        isMousePressed = false;
        QRect rf(startPtf.x(),startPtf.y(),endPtf.x()-startPtf.x(),endPtf.y()-startPtf.y());
        QPixmap tmpP = pm.copy(rf);
        QString tmpProductStyle = m_product_style;
        tmpProductStyle = tmpProductStyle.append(".png");
        tmpProductStyle.prepend("e:/template/");
        tmpP.save(tmpProductStyle);
//        tmpP.save("f:/abc.png");
        GDataFactory::get_factory()->set_current_template_image_mat(image_to_mat(pm.copy(rf)));
        emit signal_transfer_pixmap(pm.copy(rf));
        startPtf = QPointF(-100,-100);
        endPtf = QPointF(-100,-100);
    }
}

void ZoomMoveObj::slot_take_image()
{
    m_scene->removeItem(m_imageItem);
    GDataFactory::get_factory()->read_serial_number_xray(3);
    QThread::msleep(200);
    GDataFactory::get_factory()->get_xray_image();
    GDataFactory::get_factory()->read_serial_number_xray(7);

//    pm.load("f:\\1.tif");
//    m_imageItem->setPixmap(pm);
//    m_scene->addItem(m_imageItem);
//    this->setScene(m_scene);
//    this->update();
}

void ZoomMoveObj::slot_rev_pixmap(QPixmap p)
{
    if(GDataFactory::get_factory()->get_system_status() != Calibrate_Status)
        return;
    pm = p;
    m_scene->removeItem(m_imageItem);
    m_imageItem->setPixmap(p);
    m_scene->addItem(m_imageItem);
    this->setScene(m_scene);
    this->update();
}
