#ifndef BASEMYSQL_H
#define BASEMYSQL_H

#include <QSql>
#include <QSqlDatabase>
#include <QDataStream>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QObject>
#include "IDataBase.h"



class BaseMySql : public IDataBase
{

public:
    explicit BaseMySql(QObject *parent = 0);

    //connName方便连接多个数据库
    bool Open(QString dbName,QString userName,QString passWord,QString hostName="localhost",QString connName=QString());

};

#endif // BASEMYSQL_H
