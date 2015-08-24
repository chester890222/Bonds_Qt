#include "bondhandler.h"

BondHandler::BondHandler(QObject *parent) : QObject(parent)
{

}

void BondHandler::~BondHandler() {
    if (bond_db != NULL) {
        delete bond_db;
        bond_db = NULL;
    }
}

void BondHandler::Init() {

    if(!BaseWindQuant::WindAuthorize())
    {
        QMessageBox::critical(0,"ERROR:","WindQuant认证失败");
        return;
    }

    bond_db = new BaseMySql;
    bond_db->Open(IDataBase::getProperty("RISKMYSQL/DatabaseName").toString(),
                    IDataBase::getProperty("RISKMYSQL/UserName").toString(),
                    IDataBase::getProperty("RISKMYSQL/Password").toString(),
                    IDataBase::getProperty("RISKMYSQL/HostName").toString()
                    );


}
