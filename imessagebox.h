#ifndef IMESSAGEBOX_H
#define IMESSAGEBOX_H
#include <QDialog>
class QLabel;
class QTextEdit;
class IMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit IMessageBox(int infoLevel =1);
    //infoLevel:0 1 2 3 4 according to QMessageBox
//    NoIcon = 0,
//    Information = 1,
//    Warning = 2,
//    Critical = 3,
//    Question = 4
    void warning(QString strInfo);
    void error(QString strInfo);
protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *event);
    QLabel* labelInfo;
private slots:
    void slot_OK();
};

#endif // IMESSAGEBOX_H
