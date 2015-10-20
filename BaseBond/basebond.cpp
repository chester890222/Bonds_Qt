#include "basebond.h"
#include <QStringList>
#include <QDebug>

BaseBond::BaseBond(const QString &code) : BondRealtimeInfo(code) {
//    bondCode = Code;
}

BaseBond::~BaseBond() {
    qDebug() << Q_FUNC_INFO << bondCode;
}

void BaseBond::setBond_db_info(QString bType_, QString code_, QString name_,
                               QString iType_, double faceValue_,
                               QString coupons_, double paymentFrequency_,
                               double term_, QDate carryDate_, QDate listDate_,
                               QDate offlistDate_, QDate maturityDate_,
                               double issueAmount_) {
    if (bType_ == "Treasury") {
        this->bType = Treasury;
    } else if (bType_ == "LocalGov") {
        this->bType = LocalGov;
    } else if (bType_ == "Financial") {
        this->bType = Financial;
    } else if (bType_ == "Corporate") {
        this->bType = Corporate;
    } else if (bType_ == "Enterprise") {
        this->bType = Enterprise;
    } else if (bType_ == "Convertible") {
        this->bType = Convertible;
    } else if (bType_ == "Exchangeble") {
        this->bType = Exchangable;
    }
    this->code = code_;
    this->name = name_;
    if (iType_ == tr("fixed")) this->iType = fixed_interest;
    else if (iType_ == tr("floating")) this->iType = floating_interest;
    this->faceValue = faceValue_;

    this->paymentFrequency = paymentFrequency_;
    this->term = term_;
    this->carryDate = carryDate_;
    this->listDate = listDate_;
    this->offlistDate = offlistDate_;
    this->maturityDate = maturityDate_;
    this->issueAmount = issueAmount_;
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
    if (coupons_.contains(",")) {
        couponsTemp = coupons_.split(",");
    } else {
        couponsTemp = QStringList(coupons_);
    }
    if (abs(paymentFrequency_-0.0) <1e-6) {
        coupons[maturityDate] = 0.0;

    } else {
        double tmp = 12/paymentFrequency_;
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

int BaseBond::cal_last_coupon_index(QDate curDate = QDate::currentDate()) const {
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

double BaseBond::cal_currentCoupon(QDate curDate = QDate::currentDate()) const {
    QList<QDate> pay_dates = coupons.keys();
    int index = cal_last_coupon_index(curDate) + 1;
    if (index > pay_dates.size()) qDebug() << Q_FUNC_INFO << "something wrong";
    qDebug() <<"current coupon = " << coupons.value(pay_dates[index]);
    return coupons.value(pay_dates[index]);
}



double BaseBond::cal_accInterest(QDate curDate = QDate::currentDate()) const {
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

double BaseBond::cal_timeToMaturity(QDate curDate = QDate::currentDate()) const{
    int days = curDate.daysTo(maturityDate);
    return (double)days/365;
}

double BaseBond::cal_discounted_cash_flow(double cf, double rate, double time, QString Method = "Simple", double frequency = 1.0) const {
    if (Method == "Simple") {
        return cf/(1+rate*time);
    } else if (Method == "Compounded") {
        return cf/pow((1+rate/frequency),frequency*time);
    } else if (Method == "Continuous") {
        return cf*exp(-rate * time);
    } else {
        return 0.0;
    }
}

QDate BaseBond::cal_next_payment_date(QDate curDate) const {
    if (curDate.daysTo(maturityDate) <= 0) {
        return maturityDate;
    } else {
        QList<QDate> sorted_dates = coupons.keys();
        int last_ind = cal_last_coupon_index(curDate);
        return sorted_dates[last_ind+1];
    }
}


double BaseBond::cal_YTM(double price, QDate curDate = QDate::currentDate(), QString Method = "Simple") const {
    double tol = 1e-6;
    double low = -1.0, high = 1.0;
    double dis = 0;
    double p = cal_Clean_Price(dis,curDate,Method);
    while (abs(p-price)>tol) {
        if (p>price) {
            low = dis;
        }
        if (p<price) {
            high = dis;
        }
        dis = (high+low)/2;
        p = cal_Clean_Price(dis,curDate,Method);
    }

    return dis;
}

double BaseBond::cal_Clean_Price(double rate, QDate curDate = QDate::currentDate(), QString Method = "Simple") const{
    QList<QDate> pay_dates = coupons.keys();
    int last_ind = cal_last_coupon_index(curDate);
    double sum=0.0, time;

    //remaining payment times
    int times = pay_dates.size()-1-last_ind;
    //future cash flows:
    if (times == 0) {
        //Only one payment at maturity
        double p = faceValue+coupons.value(maturityDate);
        double tau = curDate.daysTo(maturityDate)/365.0;
        sum = cal_discounted_cash_flow(p,rate,tau,Method, 1.0);
    } else {
        for (int i = 1; i<=times; i++) {
        time = curDate.daysTo(pay_dates[last_ind+i])/365.0;
        sum += cal_discounted_cash_flow(coupons.value(pay_dates[last_ind+i]), rate, time, Method, 1.0);
        }
        sum += cal_discounted_cash_flow(faceValue, rate, cal_timeToMaturity(curDate), Method, 1.0);
    }
    return sum;
}

double BaseBond::cal_Dirty_Price(double rate, QDate curDate, QString Method) const {
    double clean = cal_Clean_Price(rate, curDate, Method);
    double accI = cal_accInterest(curDate);
    return accI+clean;
}
