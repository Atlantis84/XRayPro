#ifndef PROCESSPIXMAPWGT_H
#define PROCESSPIXMAPWGT_H

#include <QWidget>

class ProcessPixmapWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessPixmapWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);

    QPixmap m_pPixmap;

signals:

public slots:
    void slot_receive_pixmap(QPixmap pm);
};

#endif // PROCESSPIXMAPWGT_H
