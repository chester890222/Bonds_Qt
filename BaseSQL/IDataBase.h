#ifndef IDATABASE_H
#define IDATABASE_H

#include <QString>
#include <QVariant>
#include <QSettings>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class IDataBase:public QObject
{
    Q_OBJECT
public:
    IDataBase(QObject* parient=0)
        :QObject(parient){}
    virtual ~IDataBase()
    {
//        qDebug()<<Q_FUNC_INFO;
        Close();
    }

    static void showError(QSqlError err)
    {
        QMessageBox::critical(0, "Error",
                              "Error Database: " + err.text());
    }

    QSqlDatabase* getMyDB()
    {
        return &db;
    }

    void Close()
    {
        if(db.isOpen())
            db.close();

//        qDebug()<<Q_FUNC_INFO<<" ok";
    }

    void Transaction()
    {
        db.transaction();
    }
    void Commit()
    {
        db.commit();
    }
    void Rollback()
    {
        db.rollback();
    }


    //.ini
    static void setProperty(QString groupName,QString key,QString value)
    {
        QString fileName = "../configSql.ini";
        QSettings *settings = new QSettings(fileName,QSettings::IniFormat);
        settings->beginGroup(groupName);
        settings->setValue(key,value);
        settings->endGroup();
    }

    static QVariant getProperty(QString key)
    {
        QString fileName = "../configSql.ini";
        QSettings *settings = new QSettings(fileName,QSettings::IniFormat);
        return settings->value(key);
    }
protected:
    QSqlDatabase db;

signals:
    void signalSendMSG(QString msg,int type=21);

public slots:

};






#endif // IDATABASE_H
