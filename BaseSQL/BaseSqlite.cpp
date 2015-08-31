#include "BaseSqlite.h"

BaseSqlite::BaseSqlite(QObject* parient)
    :IDataBase(parient)
{
}
BaseSqlite::~BaseSqlite()
{

}


QSqlError BaseSqlite::Open(QString databaseName,QString connName)
{
    emit signalSendMSG("Sqlite:::>>>连接...");

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(0, "Unable to load database", "This needs the SQLITE driver");

    db = QSqlDatabase::addDatabase("QSQLITE",connName);
    db.setDatabaseName(databaseName);
    if (!db.open())
    {
        return db.lastError();
    }

    emit signalSendMSG("Sqlite:::>>>连接成功.");

    return QSqlError();
}

QStringList BaseSqlite::sqliteGetTableNames()
{
    return db.tables();
}


