#include "bondhandler.h"
#include <QDebug>
#include <QMessageBox>

BondHandler BondHandler::g_instance;

/////////////////////////////////////////////////////////////
BondHandler *BondHandler::getInstance() {
    return &g_instance;
}

BondPool *BondHandler::getBondPoolInstance() {
    return this->bondPool;
}
/////////////////////////////////////////////////////////////

BondHandler::BondHandler(QObject *parent) : QObject(parent) {
    bondPool = NULL;
    bond_db = NULL;
    isInit = FALSE;
}

BondHandler::~BondHandler() {
    clear();
}

int BondHandler::clear() {
    if (!isInit) {
        return 0;
    }

    if (bond_db != NULL) {
        //需要关闭数据库!!!!!!

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
//    qDebug() << "BONDHANDLER::INIT";
    //Wind 行情认证
    if(!BaseWindQuant::WindAuthorize()) {
        QMessageBox::critical(0,"ERROR:","WindQuant认证失败");
        return;
    }

    if (bondPool = NULL) {
        bondPool = BondPool::getInstance();
    }
    bond_db = new BaseMySql();
    bond_db->Open(IDataBase::getProperty("TreasuryBondMySQL/DatabaseName").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/UserName").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/Password").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/HostName").toString()
                  );
    selectBondFromDb();
}

void BondHandler::selectBondFromDb() {

    QString qry = QString("select tb_code from treasury_bond_info where record_date = '%1'").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QSqlQuery codes_qry(qry, *bond_db->getMyDB());
    int codesNumber = codes_qry.size();
    if (codesNumber == 0) {
        QMessageBox::critical(0,"Error:","MySQL数据库查询失败！");
    }

    QStringList codeslist;
    while (codes_qry.next()) {
        codeslist.append(codes_qry.value(0).toString());
    }

    bondPool->init(codeslist);

    qry = QString("select tb_code, tb_name, face_value, interest_type, coupons, payment_frequency, issue_amount, carry_date, maturity_date, list_date, off_list_date, record_date from treasury_bond_info where record_date = '%1'").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QSqlQuery query(qry, *bond_db->getMyDB());
//    qDebug() << "total database rows = " << recordNumber;

    int ind_tb_code = query.record().indexOf("tb_code");
    int ind_tb_name = query.record().indexOf("tb_name");
    int ind_face_value = query.record().indexOf("face_value");
    int ind_interest_type = query.record().indexOf("interest_type");
    int ind_coupons = query.record().indexOf("coupons");
    int ind_payment_frequency = query.record().indexOf("payment_frequency");
    int ind_issue_amount = query.record().indexOf("issue_amount");
    int ind_carry_date = query.record().indexOf("carry_date");
    int ind_maturity_date = query.record().indexOf("maturity_date");
    int ind_list_date = query.record().indexOf("list_date");
    int ind_off_list_date = query.record().indexOf("off_list_date");
    int ind_record_date = query.record().indexOf("record_date");

//    int colMcode  = query.record().indexOf("codeM");
//    int colAcode = query.record().indexOf("codeA");
//    int colAratio = query.record().indexOf("ratioA");
//    int colBcode = query.record().indexOf("codeB");
//    int colBratio = query.record().indexOf("ratioB");
//    QString acode;
//    double apct;
//    double bpct;
//    while(query.next())
//    {
//        acode =  query.value(colAcode).toString();
//        gLfaInfo[acode].Mcode = query.value(colMcode).toString();
//        gLfaInfo[acode].Acode = acode;
//        gLfaInfo[acode].Bcode = query.value(colBcode).toString();
//        apct = query.value(colAratio).toDouble();
//        bpct = query.value(colBratio).toDouble();
//        gLfaInfo[acode].Aratio = apct/(apct+bpct);
//        gLfaInfo[acode].Bratio = bpct/(apct+bpct);
//    }

//    if(gLfaInfo.isEmpty() || 0== gLfaInfo.count())
//        QMessageBox::critical(0,"Error:","数据库提取分级基金数据失败，无法提供配对分级A、B为权益的功能.\n\rerror info:"+query.lastError().text());

}









