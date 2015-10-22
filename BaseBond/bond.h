#ifndef BOND_H
#define BOND_H

#include <QObject>

class Bond : public QObject
{
    Q_OBJECT
public:
    explicit Bond(QObject *parent = 0);
    virtual ~Bond();

    void setBond_db_info(QString bType_, QString code_, QString name_,
                         QString iType_, double faceValue_,
                         QString coupons_, double paymentFrequency_,
                         double term_, QDate carryDate_, QDate listDate_,
                         QDate offlistDate_, QDate maturityDate_,
                         double issueAmount_);

    virtual double cal_accInterest(QDate curDate) const;
    virtual double cal_timeToMaturity(QDate curDate) const;
    virtual double cal_currentCoupon(QDate curDate) const;
    virtual int cal_last_coupon_index(QDate curDate) const;
    virtual QDate cal_next_payment_date(QDate curDate) const;

    virtual double cal_YTM(double price, QDate curDate, QString Method) const;
    virtual double cal_Clean_Price(double rate, QDate curDate, QString Method) const;
    virtual double cal_Dirty_Price(double rate, QDate curDate, QString Method) const;
    virtual double cal_discounted_cash_flow(double cf, double rate, double time, QString Method, double frequency) const;
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
signals:

public slots:
};

#endif // BOND_H
