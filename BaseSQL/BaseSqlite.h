#ifndef BASESQLITE_H
#define BASESQLITE_H


#include "IDataBase.h"

class BaseSqlite:public IDataBase
{
public:
    explicit BaseSqlite(QObject* parient=0);
    ~BaseSqlite();

    //connName方便连接多个数据库
    QSqlError Open(QString dbName=":memory:",QString connName=QString());
    QStringList sqliteGetTableNames();

private:

};

#endif // BASESQLITE_H
