#ifndef VISIONTEMPLATEWGT_H
#define VISIONTEMPLATEWGT_H

#include <QWidget>
#include <QLabel>
class VisionTemplateWgt : public QWidget
{
    Q_OBJECT
public:
    explicit VisionTemplateWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // VISIONTEMPLATEWGT_H
