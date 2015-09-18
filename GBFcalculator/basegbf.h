#ifndef BASEGBF_H
#define BASEGBF_H

#include <QObject>

class BaseGBF : public QObject
{
    Q_OBJECT
public:
    explicit BaseGBF(QObject *parent = 0);

    //realtimedata

    //base info
    //合约上市日，滚动交割起始日，集中交券日，最后交易日，配对缴款日，最后交割日


signals:

public slots:
};

#endif // BASEGBF_H
