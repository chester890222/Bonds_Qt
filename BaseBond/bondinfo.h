#ifndef BONDINFO_H
#define BONDINFO_H

#include <QObject>
#include <QMap>
#include <QDate>

enum BondType {
    Treasury=0,//tr("国债"),
    LocalGov=1,//tr("地方债"),
    Financial=2,//tr("金融债"),
    Corporate=3,//tr("公司债"),
    Enterprise=4,//tr("企业债"),
    Covertible=5,//tr("可转换债"),
    Exchangable=6 //tr("可交换债")
};

enum Market {
    SH = 0,//tr("上交所"),
    SZ = 1,//tr("深交所"),
    IB = 2,//tr("银行间")
};

enum InterestType {
    fixed=0,//tr("固定利率"),
    floating=1,//tr("浮动利率")
};

class BondInfo : public QObject
{
    Q_OBJECT
public:
    explicit BondInfo(QObject *parent = 0);

protected:
    QString Code;
    QString Name;
    BondType BType;
    InterestType IType;
    double FaceValue;
    QMap<QDate, double> Coupons;
    double PayFrequency;
    QDate CarryDate;
    QDate IPODate;
    QDate MaturityDate;
    double IssueAmount;



signals:

public slots:
};

#endif // BONDINFO_H
