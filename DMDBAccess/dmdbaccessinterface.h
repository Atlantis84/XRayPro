#ifndef DMDBACCESS_H
#define DMDBACCESS_H
#include <QtCore/qglobal.h>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include "DMDBAccess_global.h"
class DMDBACCESS_EXPORT DMDBAccessInterface
{
public:
    DMDBAccessInterface();

    /** @brief 连接数据库操作 **/
    virtual bool ConnectDataBase() = 0;

    /** @brief 连接数据库操作 **/
    virtual bool ConnectDataBaseMySQL() = 0;

    /**
     * @brief ping读关系库
    **/
    virtual bool PingReadDB() = 0;

    /**
    * @brief 获取读取数据库
    **/
    virtual QSqlDatabase& GetReadDataBase() = 0;

    /**
    * @brief 查询， 返回QSqlQueryModel的引用
    **/
    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel& model) = 0;

    /**
    * @brief 查询， 返回QSqlQueryModel的指针
    **/
    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel* model) = 0;

    /**
    * @brief 查询， 返回QSqlQuery的引用
    **/
    virtual bool GetQueryResult(const QString& strSql, QSqlQuery& sqlQuery) = 0;

    /**
    * @brief 执行sql语句
    **/
    virtual bool ExecSql(QString& strsql, QString& strErrMsg) = 0;
    static DMDBAccessInterface* GetInterface();
private:
};

//DMDBACCESS_EXPORT DMDBAccessInterface* GetDBInterface();
#endif // DMDBACCESS_H
