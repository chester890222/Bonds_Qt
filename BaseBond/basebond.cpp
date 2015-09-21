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
/*coupon dates算法
起息日=carrydate
the next payment date is determined by payment frequency
paymentfrequency = n times anually（n都是整数？目前只见到每年1、2次付息）。
next date = last_pay_date + 12 month/n，节假日顺延
(holidays are not considered here, only for weekends)
Input coupons = QString, like “2,1.123,123.12”，“1.1”
coupons are the payments on the date, not anualized!!!
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

double BaseBond::cal_currentCoupon(QDate curDate = QDate::currentDate()) {
    QList<QDate> pay_dates = coupons.keys();
    int i=0;
    while (curDate < pay_dates[i]) {
        i++;
    }
    if (i == 0) {
        qDebug() << "i=0, " << bondCode << " current coupon = " << coupons.value(pay_dates[i]);
        return coupons.value(pay_dates[i]);
    } else {
        qDebug() << coupons.value(pay_dates[i-1]);
        return coupons.value(pay_dates[i-1]);
    }
}

double BaseBond::cal_accInterest(QDate curDate = QDate::currentDate()) {
    double coupon = cal_currentCoupon(curDate), accI;
    QDate last_pay;
    QList<QDate> pay_dates = coupons.keys();
    int i=0, accDays,totalDays;
    while (curDate < pay_dates[i]) {
        i++;
    }
    if (i == 0) {
        last_pay = carryDate;
    } else {
        last_pay = pay_dates[i-1];
    }
    accDays = last_pay.daysTo(curDate);
    totalDays = last_pay.daysTo(pay_dates[i]);
    accI = coupon * (double)accDays/(double)totalDays;
    return accI;
}

double BaseBond::cal_timeToMaturity(QDate curDate = QDate::currentDate()) {
    int days = curDate.daysTo(maturityDate);
    return (double)days/365;
}


double BaseBond::cal_YTM(double price, QDate curDate = QDate::currentDate()) {

}

double BaseBond::cal_Price(double rate, QDate curDate = QDate::currentDate()) {

}
