#include "bondhandler.h"
#include <QDebug>

BondHandler::BondHandler(QObject *parent) : QObject(parent)
{
}

BondHandler::~BondHandler() {
    if (bond_db != NULL) {
        delete bond_db;
        bond_db = NULL;
    }
}

void BondHandler::Init() {
    qDebug() << "BONDHANDLER::INIT";
    //Wind 行情认证
    if(!BaseWindQuant::WindAuthorize())
    {
        QMessageBox::critical(0,"ERROR:","WindQuant认证失败");
        return;
    }

    bond_db = new BaseMySql();
    bond_db->Open(IDataBase::getProperty("Bond_MySQL/DatabaseName").toString(),
                  IDataBase::getProperty("Bond_MySQL/UserName").toString(),
                  IDataBase::getProperty("Bond_MySQL/Password").toString(),
                  IDataBase::getProperty("Bond_MySQL/HostName").toString()
                  );

}
