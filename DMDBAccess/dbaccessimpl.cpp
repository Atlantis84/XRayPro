#include "dbaccessimpl.h"
#include <QSqlError>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QDebug>
DBAccessImpl::DBAccessImpl()
{

}

bool DBAccessImpl::ConnectDataBase()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE);
    m_pDatabase.setDatabaseName(DATABASENAME);
    m_pDatabase.setPort(PORTNUM);
    m_pDatabase.setHostName(HOSTNAME);
    m_pDatabase.setUserName(USERNAME);
    m_pDatabase.setPassword(PASSWORD);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::ConnectDataBaseMySQL()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE_MYSQL);
    m_pDatabase.setDatabaseName(DATABASENAME_MYSQL);
    m_pDatabase.setPort(PORTNUM_MYSQL);
    m_pDatabase.setHostName(HOSTNAME_MYSQL);
    m_pDatabase.setUserName(USERNAME_MYSQL);
    m_pDatabase.setPassword(PASSWORD_MYSQL);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::PingReadDB()
{
    return 0;
}

QSqlDatabase &DBAccessImpl::GetReadDataBase()
{
    QSqlDatabase ss;
    return ss;
}

bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel &model)
{
    model.setQuery(strSql, m_pDatabase);
    if (model.lastError().isValid())
    {
        QMessageBox::critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel& model) failed. "
                                                     "query sql:%2. error info from database:%1")
                             .arg(model.lastError().text())
                             .arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel *model)
{
    model->setQuery(strSql, m_pDatabase);
    if (model->lastError().isValid())
    {
        QMessageBox::critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel* model) failed. "
                                                     "query sql:%2. error info from database:%1")
                             .arg(model->lastError().text())
                             .arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryResult(const QString &strSql, QSqlQuery &sqlQuery)
{

    sqlQuery = QSqlQuery(m_pDatabase);
    qDebug()<<strSql;
    if(!sqlQuery.exec(strSql))
    {
        QMessageBox::critical(NULL, "ERROR", QString("GetQueryResult(const QString& strSql, QSqlQuery& sqlQuery) failed. "
                                                     "query sql:%2. error info from database:%1")
                             .arg(sqlQuery.lastError().text())
                              .arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::ExecSql(QString &strsql, QString &strErrMsg)
{
    FormatSql(strsql);

    QSqlQuery sqlQuery = QSqlQuery(m_pDatabase);

    if(!sqlQuery.exec(strsql))
    {
        QMessageBox::critical(NULL, "ERROR", QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                                     "query sql:%2. error info from database:%1")
                             .arg(sqlQuery.lastError().text())
                              .arg(strsql));
        return false;
    }
    return true;

}

void DBAccessImpl::FormatSql(QString& strSql)
{
    strSql.trimmed();
    if(strSql[strSql.size()-1] != QChar(';'))
    {
        strSql.append(QString(";"));
    }
}
