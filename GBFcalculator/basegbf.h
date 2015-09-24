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
    explicit BaseGBF(QObject *parent = 0);


    //treasury bond info
    //合约上市日，最后交易日，滚动交割起始日，集中交券日，配对缴款日，最后交割日
    QDate listDate;
    QDate offListDate;




signals:

public slots:
};

#endif // BASEGBF_H
