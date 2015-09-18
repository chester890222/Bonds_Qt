#include "basebond.h"
#include <QStringList>
#include <QDebug>

BaseBond::BaseBond(const QString &code) : BondRealtimeInfo(code) {
//    bondCode = Code;
}

BaseBond::~BaseBond() {
    qDebug() << Q_FUNC_INFO << bondCode;
}

void BaseBond::setBond_db_info(QString BType, QString Code, QString Name,
                     QString IType, double FaceValue,
                     QString Coupons, double PaymentFrequency,
                     QDate CarryDate, QDate ListDate,
                     QDate OfflistDate,QDate MaturityDate,
                     double IssueAmount) {
    if (BType == "Treasury") {
        this->bType = Treasury;
    } else if (BType == "LocalGov") {
        this->bType = LocalGov;
    } else if (BType == "Financial") {
        this->bType = Financial;
    } else if (BType == "Corporate") {
        this->bType = Corporate;
    } else if (BType == "Enterprise") {
        this->bType = Enterprise;
    } else if (BType == "Convertible") {
        this->bType = Convertible;
    } else if (BType == "Exchangeble") {
        this->bType = Exchangable;
    }
    this->code = Code;
    this->name = Name;
    if (IType == tr("fixed")) this->iType = fixed_interest;
    else if (IType == tr("floating")) this->iType = floating_interest;
    this->faceValue = FaceValue;

    this->paymentFrequency = PaymentFrequency;
    this->carryDate = CarryDate;
    this->listDate = ListDate;
    this->offlistDate = OfflistDate;
    this->maturityDate = MaturityDate;
    this->issueAmount = IssueAmount;
/*######################### coupons算法！！
起息日=carrydate
下一次付息日由payment frequency决定，1年n次付息（n都是整数？目前只见到每年1、2次付息）。
next date = carrydate + month/n个月，节假日顺延
input coupons = QString, “2,1.123,123.12”，“1.1”
*/
    QStringList couponsTemp;
    int i=0;
    if (Coupons.contains(",")) {
        couponsTemp = Coupons.split(",");
    } else {
        couponsTemp = QStringList(Coupons);
    }
    double tmp = 12/PaymentFrequency;
    if (tmp == (int)tmp) {
        QDate next = carryDate, next_temp;
        while (next <= maturityDate) {
            next = next.addMonths(12/PaymentFrequency);
            if (next.dayOfWeek() > 5) {
                next_temp = next.addDays(8-next.dayOfWeek());
            } else {
                next_temp = next;
            }
            if (couponsTemp.size() > 1) {
                coupons[next_temp] =couponsTemp.at(i).toDouble();
            } else {
                coupons[next_temp] = couponsTemp.at(0).toDouble();
            }
            i++;
//            qDebug() << code << next_temp << coupons[next_temp];
        }
    }

}

double BaseBond::cal_accInterest() {
    return 0.0;
}

double BaseBond::cal_currentCoupon() {
    return 0.0;
}

double BaseBond::cal_timeToMaturity() {
    return 0.0;
}


void BaseBond::cal_YTM(double price, QDate CurDate) {

}
