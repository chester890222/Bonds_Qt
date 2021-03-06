#include "bondhandler.h"
#include <QMessageBox>

#include <QDebug>

BondHandler *BondHandler::g_instance;

/////////////////////////////////////////////////////////////
BondHandler *BondHandler::getInstance() {
    return g_instance;
}

BondPool *BondHandler::getBondPoolInstance() {
    return this->bondPool;
}
/////////////////////////////////////////////////////////////

BondHandler::BondHandler(QObject *parent) : QObject(parent) {
    qDebug() <<Q_FUNC_INFO;
    bondPool = NULL;
    bond_db = NULL;
    isInit = FALSE;
    g_instance = this;
}

BondHandler::~BondHandler() {
    qDebug()<<Q_FUNC_INFO;
    clear();
}

int BondHandler::clear() {
    qDebug()<<Q_FUNC_INFO;

    if (bond_db != NULL) {
        delete bond_db;
        bond_db = NULL;
    }
    if (bondPool != NULL) {
//        bondPool->clear();
        delete bondPool;
        bondPool = NULL;
    }
    return 0;
}


void BondHandler::init() {
    qDebug()<<Q_FUNC_INFO;
    //Wind 行情认证
    if(!BaseWindQuant::WindAuthorize()) {
        QMessageBox::critical(0,"ERROR:","WindQuant认证失败");
        return;
    }

    //连接数据库
    bond_db = new BaseMySql();
    bond_db->Open(IDataBase::getProperty("TreasuryBondMySQL/DatabaseName").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/UserName").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/Password").toString(),
                  IDataBase::getProperty("TreasuryBondMySQL/HostName").toString()
                  );

//    QStringList codeList = getBondCodes();
    QStringList codeList;
    codeList << "150011.IB" << "150012.IB";

    bondPool = new BondPool();

    bondPool->init(codeList);

    selectBondFromDb(codeList);

    QString indicators("rt_last,rt_last_vol,rt_bid1,rt_ask1,rt_bsize1,rt_asize1");

    bondPool->requestDataFromWind(indicators,true);
}

QStringList BondHandler::getBondCodes() {
    qDebug() << Q_FUNC_INFO;
    QStringList codeslist;
    QString qry = QString("select code from treasury_bond_info where record_date = '%1'").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QSqlQuery codes_qry(qry, *bond_db->getMyDB());
    int codesNumber = codes_qry.size();
    if (codesNumber == 0) {
        QMessageBox::critical(0,"Error:","MySQL error: no value are selected!");
        return codeslist;
    }
    while (codes_qry.next()) {
        codeslist.append(codes_qry.value(0).toString());
    }

    return codeslist;
}

void BondHandler::selectBondFromDb() {
    qDebug() << Q_FUNC_INFO;

    QString qry = QString("select code, name, face_value, interest_type, coupons, payment_frequency, issue_amount, term, carry_date, maturity_date, list_date, off_list_date, record_date from treasury_bond_info where record_date = '%1'").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QSqlQuery query(qry, *bond_db->getMyDB());

    int ind_code = query.record().indexOf("code");
    int ind_name = query.record().indexOf("name");
    int ind_face_value = query.record().indexOf("face_value");
    int ind_interest_type = query.record().indexOf("interest_type");
    int ind_coupons = query.record().indexOf("coupons");
    int ind_payment_frequency = query.record().indexOf("payment_frequency");
    int ind_issue_amount = query.record().indexOf("issue_amount");
    int ind_term = query.record().indexOf("term");
    int ind_carry_date = query.record().indexOf("carry_date");
    int ind_maturity_date = query.record().indexOf("maturity_date");
    int ind_list_date = query.record().indexOf("list_date");
    int ind_off_list_date = query.record().indexOf("off_list_date");
    int ind_record_date = query.record().indexOf("record_date");
    QString code;
    while (query.next()) {
        code = query.value(ind_code).toString();
        bondPool->bondMap->value(code)->setBond_db_info("Treasury",
                                                 query.value(ind_code).toString(),
                                                 query.value(ind_name).toString(),
                                                 query.value(ind_interest_type).toString(),
                                                 query.value(ind_face_value).toDouble(),
                                                 query.value(ind_coupons).toString(),
                                                 query.value(ind_payment_frequency).toDouble(),
                                                 query.value(ind_term).toDouble(),
                                                 query.value(ind_carry_date).toDate(),
                                                 query.value(ind_list_date).toDate(),
                                                 query.value(ind_off_list_date).toDate(),
                                                 query.value(ind_maturity_date).toDate(),
                                                 query.value(ind_issue_amount).toDouble());

    }
}

void BondHandler::selectBondFromDb(QStringList codesList) {
    qDebug() << Q_FUNC_INFO;
    QString codes = codesList.join("','");
    QString qry = QString("select code, name, face_value, interest_type, coupons, payment_frequency, issue_amount, term, carry_date, maturity_date, list_date, off_list_date, record_date from treasury_bond_info where code in ('%1')").arg(codes);
    QSqlQuery query(qry, *bond_db->getMyDB());

    int ind_code = query.record().indexOf("code");
    int ind_name = query.record().indexOf("name");
    int ind_face_value = query.record().indexOf("face_value");
    int ind_interest_type = query.record().indexOf("interest_type");
    int ind_coupons = query.record().indexOf("coupons");
    int ind_payment_frequency = query.record().indexOf("payment_frequency");
    int ind_issue_amount = query.record().indexOf("issue_amount");
    int ind_term = query.record().indexOf("term");
    int ind_carry_date = query.record().indexOf("carry_date");
    int ind_maturity_date = query.record().indexOf("maturity_date");
    int ind_list_date = query.record().indexOf("list_date");
    int ind_off_list_date = query.record().indexOf("off_list_date");
    int ind_record_date = query.record().indexOf("record_date");
    QString code;
    while (query.next()) {
        code = query.value(ind_code).toString();
        bondPool->bondMap->value(code)->setBond_db_info("Treasury",
                                                 query.value(ind_code).toString(),
                                                 query.value(ind_name).toString(),
                                                 query.value(ind_interest_type).toString(),
                                                 query.value(ind_face_value).toDouble(),
                                                 query.value(ind_coupons).toString(),
                                                 query.value(ind_payment_frequency).toDouble(),
                                                 query.value(ind_term).toDouble(),
                                                 query.value(ind_carry_date).toDate(),
                                                 query.value(ind_list_date).toDate(),
                                                 query.value(ind_off_list_date).toDate(),
                                                 query.value(ind_maturity_date).toDate(),
                                                 query.value(ind_issue_amount).toDouble());

    }
}










