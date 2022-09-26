#ifndef PRODUCTSTYLEDLG_H
#define PRODUCTSTYLEDLG_H

#include <QDialog>
#include <QComboBox>
class ProductStyleDlg : public QDialog
{
    Q_OBJECT
public:
    ProductStyleDlg();
private:
    QComboBox* m_pCmbProductStyle;
    void paintEvent(QPaintEvent *event);

public slots:
    void slot_comb_index_changed(int index);
    void slot_ok_click();
};

#endif // PRODUCTSTYLEDLG_H
