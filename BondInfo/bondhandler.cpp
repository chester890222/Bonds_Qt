#include "bondhandler.h"
#include <QDebug>

BondHandler::BondHandler(QObject *parent) : QObject(parent)
{
}

BondHandler::~BondHandler() {
    clear();
}

int BondHandler::clear() {

    if (bond_db != NULL) {
        delete bond_db;
        bond_db = NULL;
    }
    if (bondPool != NULL) {
        bondPool->cancelRequestFromWind();
        bondPool->clear();
        bondPool = NULL;
    }

    return 0;
}


void BondHandler::init() {
    qDebug() << "BONDHANDLER::INIT";
    //Wind 行情认证
    if(!BaseWindQuant::WindAuthorize())
    {
        QMessageBox::critical(0,"ERROR:","WindQuant认证失败");
        return;
    }

    if (bondPool = NULL) {
        bondPool = new BondPool();
    }
    bond_db = new BaseMySql();
    bond_db->Open(IDataBase::getProperty("Bond_MySQL/DatabaseName").toString(),
                  IDataBase::getProperty("Bond_MySQL/UserName").toString(),
                  IDataBase::getProperty("Bond_MySQL/Password").toString(),
                  IDataBase::getProperty("Bond_MySQL/HostName").toString()
                  );
    selectFromBondDb();

}

void BondHandler::selectBondFromDb() {
    QString qry("select tb_code, tb_name, face_value, interest_type, coupons, payment_frequency, issue_amount, carry_date, maturity_date, list_date, off_list_date, record_date from treasury_bond_info where record_date =",QDate::currentDate().toString("YYYY-MM-DD"));
    qDebug() <<qry;
    //    QSqlQuery query(qry, *bond_db->getMyDB());


}


BondPool *BondHandler::getBondPoolInstance() {
    return this->bondPool;
}







