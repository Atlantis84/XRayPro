#include "testwindow.h"
#include "gdatafactory.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "split.h"
#include "QsLog.h"
TestWindow::TestWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QHBoxLayout* hBox2 = new QHBoxLayout();

    this->resize(QSize(800,600));
    this->setStyleSheet("font-family:'FangSong';font-size:20px;");
    leNumber = new QLineEdit(this);
    leLog = new QLineEdit(this);
    QPushButton* btnTakePhoto = new QPushButton(u8"ÅÄÕÕ¶ÁÂë²âÊÔ",this);
    connect(btnTakePhoto,SIGNAL(clicked()),this,SLOT(slot_take_photo()));
    hBox1->addWidget(leNumber);
    hBox1->addWidget(btnTakePhoto);
    hBox2->addWidget(leLog);
    vAll->addStretch();
    vAll->addLayout(hBox1);
    vAll->addLayout(hBox2);
    this->setLayout(vAll);
}

void TestWindow::slot_take_photo()
{
    while(1)
    {
        cv::Mat image;
        GDataFactory::get_camera_interface()->StartGrabbing();
        GDataFactory::get_camera_interface()->SoftwareTriggerOnce();
        image = GDataFactory::get_camera_interface()->GetImage();
        if (!image.empty() && image.data)
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyyMMddhhmmss");
            QString imageName = current_date +".jpg";
            cv::imwrite(imageName.toStdString(),image);
//            GDataFactory::get_factory()->mat_to_pixmap(image);
            break;
        }
    }

}
