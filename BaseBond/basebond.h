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

    void setBond_db_info(QString bType, QString code, QString name,
                         QString iType, double faceValue,
                         QString coupons, double paymentFrequency,
                         QDate carryDate, QDate listDate,
                         QDate offlistDate,QDate maturityDate,
                         double issueAmount);

    double cal_accInterest();
    double cal_timeToMaturity();
    double cal_currentCoupon();

    void cal_YTMs();

protected:

//    Bond_db_info bond_db_info;
//    BondRealtimeInfo bondRealtimeInfo;

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
