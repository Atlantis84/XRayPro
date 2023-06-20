#ifndef PRODUCTMANAGEWGT_H
#define PRODUCTMANAGEWGT_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <ActiveQt/QAxObject>
#include <QThread>
#include <QDateEdit>
class ImportThread: public QThread
{
    Q_OBJECT
public:
    explicit ImportThread(QWidget *parent = nullptr){}

private:
    void run();
signals:
    void signal_import_over();
};

class ProductManageWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ProductManageWgt(QWidget *parent = nullptr);

private:
    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void set_table_data();
    void set_table_data_query();
    QString open_excel_file();
    QLineEdit* leSecondSecCode;
    QLineEdit* leProductStyle;
    QComboBox* cmb_product_style;

    QAxObject *m_WorkBook_Confirm;
    QAxObject *m_WorkSheets_Confirm;
    QAxObject *m_WorkSheet_Confirm;
    QAxObject *m_Range_Confirm;
    QAxObject *m_Row_Confirm;
    QAxObject *m_Column_Confirm;
    int nRows_Confirm;
    int nColumns_Confirm;
    QAxObject *m_Cell_Confirm;
    struct Struct_Confirm
    {
        QString m_Column_1;
        QString m_Column_2;
        QString m_Column_3;
    };
    QVector<Struct_Confirm> m_Struct_Confirm;

    ImportThread* m_pImportThread;

    QDateEdit* m_pDateEdit;
signals:

public slots:
    void slot_query();
    void slot_add();
    void slot_delete();
    void slot_import();
    void slot_import_over();
    void slot_export();
    void slot_export_count_result();
};

#endif // PRODUCTMANAGEWGT_H
