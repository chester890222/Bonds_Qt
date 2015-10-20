#ifndef BASEBOND_H
#define BASEBOND_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QMap>
#include "bondrealtimeinfo.h"

//债券种类
enum BondType {
    Treasury=0,//tr("国债"),
    LocalGov=1,//tr("地方债"),
    Financial=2,//tr("金融债"),
    Corporate=3,//tr("公司债"),
    Enterprise=4,//tr("企业债"),
    Convertible=5,//tr("可转换债"),
    Exchangable=6 //tr("可交换债")
};

//交易场所
enum Market {
    SH = 0,//tr("上交所"),
    SZ = 1,//tr("深交所"),
    IB = 2,//tr("银行间")
};

//利率类型
enum InterestType {
    fixed_interest=0,//tr("固定利率"),
    floating_interest=1,//tr("浮动利率")
};


class Bond_db_info {
public:
    BondType bType;

    QString code;
    QString name;
    InterestType iType;
    double faceValue;
    QMap<QDate, double> coupons;
    double paymentFrequency;
    double term;
    QDate carryDate;
    QDate listDate;
    QDate offlistDate;
    QDate maturityDate;
    double issueAmount;

};


class BaseBond : public BondRealtimeInfo, public Bond_db_info
{
    Q_OBJECT
public:
    BaseBond(const QString &code);
    ~BaseBond();

    void setBond_db_info(QString bType_, QString code_, QString name_,
                         QString iType_, double faceValue_,
                         QString coupons_, double paymentFrequency_,
                         double term_, QDate carryDate_, QDate listDate_,
                         QDate offlistDate_, QDate maturityDate_,
                         double issueAmount_);

    double cal_accInterest(QDate curDate) const;
    double cal_timeToMaturity(QDate curDate) const;
    double cal_currentCoupon(QDate curDate) const;
    int cal_last_coupon_index(QDate curDate) const;
    QDate cal_next_payment_date(QDate curDate) const;

    double cal_YTM(double price, QDate curDate, QString Method) const;
    double cal_Clean_Price(double rate, QDate curDate, QString Method) const;
    double cal_Dirty_Price(double rate, QDate curDate, QString Method) const;
    double cal_discounted_cash_flow(double cf, double rate, double time, QString Method, double frequency) const;
protected:

    double rt_vwap_YTM;
    double rt_high_YTM;
    double rt_low_YTM;
    double rt_last_YTM;
    double rt_latest_YTM;
    double rt_bid_YTM[5];
    double rt_ask_YTM[5];

    double accInterest;
    double timeToMaturity;
    double currentCoupon;


};

#endif // BASEBOND_H
