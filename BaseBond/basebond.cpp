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
    if (abs(PaymentFrequency-0.0) <1e-6) {
        coupons[maturityDate] = 0.0;

    } else {
        double tmp = 12/PaymentFrequency;
        if (tmp == (int)tmp) {
            QDate next = carryDate, next_temp;
            while (next <= maturityDate) {
                next = next.addMonths((int)tmp);
                if (next.dayOfWeek() > 5) {
                    next_temp = next.addDays(8-next.dayOfWeek());
                } else {
                    next_temp = next;
                }
                if (couponsTemp.size() > 1) {
                    coupons[next_temp] =couponsTemp.at(i).toDouble()/tmp;
                } else {
                    coupons[next_temp] = couponsTemp.at(0).toDouble()/tmp;
                }
                i++;
//            qDebug() << code << next_temp << coupons[next_temp];
            }
        } else {
            qDebug() << "12/paymentfrequncy is not integer! double check!!" << bondCode;
        }
    }
}

int BaseBond::cal_last_coupon_index(QDate curDate = QDate::currentDate()) {
    // return -1 : no coupon paid yet
    QList<QDate> pay_dates = coupons.keys();
    int days=-100000000,tmp_days, index=-1;
    for (int i=0; i < pay_dates.size(); i++) {
        tmp_days = curDate.daysTo(pay_dates[i]);
        if (tmp_days > days && tmp_days < 0) {
            days = tmp_days;
            index = i;
        }
    }
    return index;

}

double BaseBond::cal_currentCoupon(QDate curDate = QDate::currentDate()) {
    QList<QDate> pay_dates = coupons.keys();
    int index = cal_last_coupon_index(curDate) + 1;
    if (index > pay_dates.size()) qDebug() << Q_FUNC_INFO << "something wrong";
    qDebug() <<"current coupon = " << coupons.value(pay_dates[index]);
    return coupons.value(pay_dates[index]);
}



double BaseBond::cal_accInterest(QDate curDate = QDate::currentDate()) {
    double coupon = cal_currentCoupon(curDate), accI;
    int last_pay_ind = cal_last_coupon_index(curDate);
    int accDays, totalDays;
    QList<QDate> pay_dates = coupons.keys();
    QDate last_pay;
    if (last_pay_ind == -1)
        last_pay = carryDate;
    else
        last_pay = pay_dates[last_pay_ind];

    accDays = last_pay.daysTo(curDate);
    totalDays = last_pay.daysTo(pay_dates[last_pay_ind+1]);
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
    QList<QDate> pay_dates = coupons.keys();
    int last_ind = cal_last_coupon_index(curDate);
    double accI = cal_accInterest(curDate);
    int tmp_days;
    double sum=0.0, cashflow, time, discount;
    //calculate discounted future values on next payment day
//    if (pay_dates.size() > last_ind+1)
//    for (int i = last_ind+1; i<=pay_dates.size();i++) {
//        time = pay_dates[last_ind+1].daysTo(pay_dates[i]);
//    }

}
