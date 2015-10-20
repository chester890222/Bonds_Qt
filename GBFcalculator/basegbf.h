#ifndef BASEGBF_H
#define BASEGBF_H

#include <QObject>
#include <QDate>
#include "basebond.h"
#include "bondrealtimeinfo.h"

class BaseGBF : public QObject, BondRealtimeInfo
{
    Q_OBJECT
public:
    BaseGBF(const QString &code);

    QDate getDate(QString type);
    void setDate(QString type, QDate date);
    QMap<QString, double> getConvFactors();
    QMap<QString, BaseBond*>* getDeliBonds();


protected:
    //treasury bond info
    QDate listDate; //上市日
    QDate offListDate; //最后交易日
    QDate firstDeliveryDate; //滚动交割起始日
    QDate lastDeliveryDate; //最后交券日
    QDate lastMatchDate; //配对缴款日
    QDate lastReceiveDate; //最后收券日
    QMap<QString, double> conversion_factors;
    QMap<QString, BaseBond*> deliverable_bonds;



signals:

public slots:
};

#endif // BASEGBF_H
