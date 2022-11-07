#include "productmanagewgt.h"
#include "title/TitleBar.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>
#include "imessagebox.h"
#include <QPushButton>
#include <QGroupBox>
#include "gdatafactory.h"
#include <QShowEvent>
#include "QsLog.h"
#include <QFileDialog>
static QString str_excel_path;
ProductManageWgt::ProductManageWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(600,600);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"盘料产品管理");
    pTitleBar->setFixedWidth(600);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();

    vBox1->addWidget(createTableWidget());
    QGroupBox* grpAllData = new QGroupBox(u8"物料信息");
    grpAllData->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpAllData->setLayout(vBox1);

    QLabel* labelSecondSecCode = new QLabel(u8"四段码-2#码:");
    leSecondSecCode = new QLineEdit();
    leSecondSecCode->setAlignment(Qt::AlignCenter);
    leSecondSecCode->setStyleSheet("min-width:150px;max-width:150px;");
    QRegExp regx("[A-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,leSecondSecCode);
    leSecondSecCode->setValidator(validator);
    QLabel* labelProductStyle = new QLabel(u8"   产品类型:");
//    cmb_product_style = new QComboBox();
    leProductStyle = new QLineEdit();
    leProductStyle->setAlignment(Qt::AlignCenter);
    leProductStyle->setStyleSheet("min-width:150px;max-width:150px;");

    QGroupBox* grpConfigData = new QGroupBox(u8"配置信息");
    QPushButton* btnQuery = new QPushButton(u8" 查  询 ");
    connect(btnQuery,SIGNAL(clicked()),this,SLOT(slot_query()));
    QPushButton* btnAdd = new QPushButton(u8" 添  加 ");
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(slot_add()));
    QPushButton* btnImport = new QPushButton(u8" 表 格 导 入 ");
//    btnImport->setStyleSheet("min-height:50px;max-height:50px;");
    connect(btnImport,SIGNAL(clicked()),this,SLOT(slot_import()));
    QPushButton* btnDelete = new QPushButton(u8" 删 除 数 据 ");
//    btnDelete->setStyleSheet("min-height:50px;max-height:50px;");
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(slot_delete()));
    grpConfigData->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    hBox1->addWidget(labelSecondSecCode);
    hBox1->addWidget(leSecondSecCode);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(labelProductStyle);
    hBox2->addWidget(leProductStyle);
    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addLayout(hBox1);
    vBox2->addLayout(hBox2);

    QVBoxLayout* vBox3 = new QVBoxLayout();
    vBox3->addWidget(btnQuery);
    vBox3->addWidget(btnAdd);

    QVBoxLayout* vBox4 = new QVBoxLayout();
    vBox4->addWidget(btnImport);
    vBox4->addWidget(btnDelete);

    QHBoxLayout* hBoxAll = new QHBoxLayout();
    hBoxAll->addStretch();
    hBoxAll->addLayout(vBox2);
    hBoxAll->addLayout(vBox3);
    hBoxAll->addLayout(vBox4);
    hBoxAll->addStretch();
    grpConfigData->setLayout(hBoxAll);

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpAllData);
//    vAll->addWidget(createTableWidget());
    vAll->addWidget(grpConfigData);
    this->setLayout(vAll);

    m_pImportThread = new ImportThread();
    connect(m_pImportThread,SIGNAL(signal_import_over()),this,SLOT(slot_import_over()));
}

QWidget *ProductManageWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(10000,2);

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"四段码-2#码"<<u8"产品类型";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    return tableWidget;
}

void ProductManageWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void ProductManageWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

//    cmb_product_style->clear();
//    QMap<QString,QString> tmpProductInfo;
//    tmpProductInfo = GDataFactory::get_factory()->get_product_info();
//    QMap<QString,QString>::Iterator itr = tmpProductInfo.begin();
//    QList<QString> lstProductStyle;
//    while(itr != tmpProductInfo.end())
//    {
//        if(lstProductStyle.contains(itr.value()))
//        {
//            itr++;
//            continue;
//        }
//        lstProductStyle.push_back(itr.value());
//        itr++;
//    }

//    for(int i=0;i<lstProductStyle.length();i++)
//        cmb_product_style->insertItem(i,lstProductStyle[i]);

//    cmb_product_style->setCurrentIndex(0);

    set_table_data();
}

void ProductManageWgt::set_table_data()
{
    this->tableWidget->clearContents();

    QMap<QString,QString> tmpProductInfo;
    tmpProductInfo = GDataFactory::get_factory()->get_product_info();
    QMap<QString,QString>::Iterator itr = tmpProductInfo.begin();
    int itemCount = 0;
    while(itr != tmpProductInfo.end())
    {
        QTableWidgetItem* item1 = new QTableWidgetItem(itr.key());
        QTableWidgetItem* item2 = new QTableWidgetItem(itr.value());
        item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
        item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));

        tableWidget->setItem(itemCount,0,item1);
        tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
        tableWidget->setItem(itemCount,1,item2);
        tableWidget->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        tableWidget->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);
        itr++;
        itemCount++;
    }
}

void ProductManageWgt::set_table_data_query()
{
    this->tableWidget->clearContents();

    QMap<QString,QString> tmpProductInfo;
    QString strSql;
    if((this->leSecondSecCode->text().trimmed() != "") && (this->leProductStyle->text().trimmed() != ""))
        strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2' and \"Second_Section_Code\"='%3'").
                arg(constProductStyleMapTable).arg(this->leProductStyle->text()).arg(this->leSecondSecCode->text());
    else
    {
        if(this->leProductStyle->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2'").
                    arg(constProductStyleMapTable).arg(this->leProductStyle->text());
        else if(this->leSecondSecCode->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"Second_Section_Code\"='%2'").
                    arg(constProductStyleMapTable).arg(this->leSecondSecCode->text());
        else
            strSql = QString("select * from public.%1").
                    arg(constProductStyleMapTable);
    }


    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        while(queryResult.next())
        {
            tmpProductInfo.insert(queryResult.value(0).toString(),queryResult.value(1).toString());
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }

    QMap<QString,QString>::Iterator itr = tmpProductInfo.begin();
    int itemCount = 0;
    while(itr != tmpProductInfo.end())
    {
        tableWidget->setItem(itemCount,0,new QTableWidgetItem(itr.key()));
        tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
        tableWidget->setItem(itemCount,1,new QTableWidgetItem(itr.value()));
        tableWidget->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);
        itr++;
        itemCount++;
    }
}

void ProductManageWgt::slot_query()
{
    set_table_data_query();
}

void ProductManageWgt::slot_add()
{
    IMessageBox* msg = new IMessageBox(3);
    if((this->leSecondSecCode->text() == "") || (this->leProductStyle->text() == ""))
    {
        msg->warning(u8"添加操作，参数不允许为空!");
        return;
    }

    //set sender para
    QString strSql = QString("select * from public.%1 where \"Second_Section_Code\"='%2'").
            arg(constProductStyleMapTable).arg(this->leSecondSecCode->text());
    QSqlQuery queryResult;
    QString strError;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_INFO()<<"query database SUCCESS!";
        if(queryResult.size() == 0)//insert new row
        {
            strSql = QString("insert into %1 values('%2','%3')").
                    arg(constProductStyleMapTable).arg(this->leSecondSecCode->text()).
                    arg(this->leProductStyle->text());
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
            {
                QLOG_INFO()<<"insert into DB SUCCESS!";
            }
            else
            {
                QLOG_WARN()<<"insert into DB FAILED!";
            }
        }
        else//update current row
        {
            msg->warning(u8"数据库中已存在该物料!");
            QLOG_WARN()<<"current info already exist in the DB";
        }
    }
    else{
        QLOG_WARN()<<"query database FAILED!";
    }

    set_table_data();
}

void ProductManageWgt::slot_delete()
{
//    QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();

//    if(items.size() == 0)
//    {
//        IMessageBox* msgBox = new IMessageBox(3);
//        msgBox->warning(u8"请选中要删除的行!");
//        return;
//    }

    QString strCode,strStyle;
//    for(int i=0;i<items.count();i++)
//    {
//        QTableWidgetItem* item = items.at(i);
//        if(i==0)
//            strCode = item->text();
//        else if(i==1)
//            strStyle = item->text();
//        else
//        {
//            return;
//        }
//    }
    strCode = leSecondSecCode->text();
    strStyle = leProductStyle->text();

    QString strSql = QString("delete from %1 where \"Second_Section_Code\" = '%2' and \"Product_Style_Code\" = '%3'").
            arg(constProductStyleMapTable).
            arg(strCode).
            arg(strStyle);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
    {
        QLOG_INFO()<<"delete from user table SUCCESS!";
        set_table_data();
    }
    else{
        QLOG_WARN()<<"delete from user table FAILED!";
    }
}

QString ProductManageWgt::open_excel_file()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(u8"产品信息文件");//
    fileDialog->setNameFilter("Conf(*.xls *.xlsx)");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog->exec()){
       fileNames = fileDialog->selectedFiles();
    }

    if(fileNames.size()==0){
        return "";
    }else{
        return fileNames.first();
    }
}

static void insert_info_to_db(QString sectioncode, QString productmodel)
{
    QString strSql = QString("select * from public.%1 where \"Product_Style_Code\"='%2' and \"Second_Section_Code\"='%3'").
            arg(constProductStyleMapTable).arg(productmodel).arg(sectioncode);
    QSqlQuery queryResult;
    QString strError;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() == 0)//insert new row
        {
            strSql = QString("insert into %1 values('%2','%3')").
                    arg(constProductStyleMapTable).arg(sectioncode).
                    arg(productmodel);
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
            {
                ;
            }
            else
            {
                QLOG_WARN()<<"insert into DB FAILED!";
            }
        }
        else
        {
            ;
        }
    }
    else
        ;
}

void ProductManageWgt::slot_import()
{
    str_excel_path = open_excel_file();
    QFile m_file_Confirm(str_excel_path);
    if(!m_file_Confirm.exists())
    {
        IMessageBox* msg = new IMessageBox(3);
        msg->warning(u8"所选文件不存在!");
        return;
    }

    GDataFactory::get_wait_count_wgt()->show();
    m_pImportThread->start();
}

void ImportThread::run()
{
    while(1)
    {
        QAxObject *m_Excel = new QAxObject(this);
        m_Excel->setControl("Excel.Application");
        m_Excel->dynamicCall("SetVisible(bool Visible)" , "false");
        m_Excel->setProperty("DisplayAlerts", false);

        QAxObject *m_WorkBooks = m_Excel->querySubObject("WorkBooks");
        QAxObject *workBook = m_WorkBooks->querySubObject("Open(const QString&)", str_excel_path);
        QAxObject *workSheet = workBook->querySubObject("WorkSheets(int)", 1);
        QAxObject *Range = workSheet->querySubObject("UsedRange");

//        int rows = Range->querySubObject ("Rows")->property ("Count").toInt ();
//        int cols = Range->querySubObject ("Columns")->property ("Count").toInt ();
//        QLOG_INFO()<<"rows:"<<rows<<", cols:"<<cols;

        QVariant var = Range->dynamicCall("Value");
        int j=0;
        foreach(QVariant varRow,var.toList()){
            if(j != 0)
            {
                int i=0;
                QString sectionCode;
                QString productModel;
                foreach(QVariant var,varRow.toList()){
                    if(i==0)
                        sectionCode = var.toString();
                    else
                        productModel = var.toString();
                    i++;
                }
                insert_info_to_db(sectionCode,productModel);
            }
            j++;
        }

        emit signal_import_over();
        workBook->dynamicCall("Close(Boolean)",false);
        m_Excel->dynamicCall("Quit(void)",false);
        if(m_Excel)
        {
            delete m_Excel;
            m_Excel = nullptr;
        }
        break;
    }
}

void ProductManageWgt::slot_import_over()
{
    GDataFactory::get_wait_count_wgt()->hide();
    m_pImportThread->terminate();
}
