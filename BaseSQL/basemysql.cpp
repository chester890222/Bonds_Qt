#include "BaseMySql.h"
#include <QDateTime>

BaseMySql::BaseMySql(QObject *parent) :
    IDataBase(parent)
{
    if(db.isOpen())
        db.close();
}

bool BaseMySql::Open(QString dbName,
                          QString userName,QString passWord,
                          QString hostName,QString connName)
{
    emit signalSendMSG("MySql:::>>>连接...");

    if (!QSqlDatabase::drivers().contains("QMYSQL"))
        QMessageBox::critical(0, "Unable to load database", "This needs the QMYSQL driver");

    db = QSqlDatabase::addDatabase("QMYSQL",connName);
    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(passWord);
    if (!db.open())
    {
        QMessageBox::information(0,"提示:",db.lastError().text());

        return false;
    }

    emit signalSendMSG("MySql:::>>>连接成功.");

    return true;
}
