#ifndef BASEBOND_H
#define BASEBOND_H

#include <QString>
#include <QDate>
#include <QMap>

class BondRealtimeData {
//债券实时数据（YTM由最新数据计算）
public:
    QString windCode;
    double date_db;

    double rt_vwap; //日内均价
    double rt_vwap_YTM;
    double rt_high; //最高价
    double rt_high_YTM;
    double rt_low;  //最低价
    double rt_low_YTM;
    double rt_last; //现价
    double rt_last_YTM;
    double rt_last_amt; //现额
    double rt_last_vol; //现量
    double rt_latest; //最新成交价
    double rt_latest_YTM;
    double rt_vol; //成交量
    double rt_amt; //成交额

    double rt_bid[5]; //买一至买五价，数组下标0表示买一，1表示买二，以此类推
    double rt_bid_YTM[5];
    double rt_ask[5]; //卖价
    double rt_ask_YTM[5];
    double rt_bsize[5]; //买量
    double rt_asize[5]; //卖量
};

//债券种类
enum BondType {
    Treasury=0,//tr("国债"),
    LocalGov=1,//tr("地方债"),
    Financial=2,//tr("金融债"),
    Corporate=3,//tr("公司债"),
    Enterprise=4,//tr("企业债"),
    Covertible=5,//tr("可转换债"),
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
    QDate MaturityDate;
    double IssueAmount;

};


class BaseBond
{

public:
    BaseBond();
};

#endif // BASEBOND_H
