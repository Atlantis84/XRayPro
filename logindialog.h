#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QLineEdit>
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog();

private:
    QLineEdit* m_pLineAccount;
    void paintEvent(QPaintEvent *event);

public slots:
    void slot_ok();
    void slot_cancel();
};

#endif // LOGINDIALOG_H
