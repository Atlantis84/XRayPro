#ifndef DBACCESSIMPL_H
#define DBACCESSIMPL_H

#include "dmdbaccessinterface.h"

class DBAccessImpl : public DMDBAccessInterface
{
public:
    DBAccessImpl();
    virtual bool ConnectDataBase();
    virtual bool ConnectDataBaseMySQL();
    virtual bool PingReadDB();
    virtual QSqlDatabase& GetReadDataBase();
    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel& model);
    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel* model);
    virtual bool GetQueryResult(const QString& strSql, QSqlQuery& sqlQuery);
    virtual bool ExecSql(QString& strsql, QString& strErrMsg);
private:
    QSqlDatabase m_pDatabase;
    void FormatSql(QString& strSql);
};

#endif // DBACCESSIMPL_H
