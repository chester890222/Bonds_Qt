#ifndef BONDREALTIMEDATA_H
#define BONDREALTIMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>
#include <QDateTime>
#include <QReadWriteLock>
#include "BaseWindQuant.h"

class BondRealtimeData {
//债券实时数据
public:
    QString windCode;
    double date_db;

    double rt_vwap; //日内均价
    double rt_high; //最高价
    double rt_low;  //最低价
    double rt_last; //现价
    double rt_last_amt; //现额
    double rt_last_vol; //现量
    double rt_latest; //最新成交价
    double rt_vol; //成交量
    double rt_amt; //成交额

    double rt_bid[5]; //买一至买五价，数组下标0表示买一，1表示买二，以此类推
    double rt_ask[5]; //卖价
    double rt_bsize[5]; //买量
    double rt_asize[5]; //卖量
};



class BondRealtimeInfo : public BaseWindQuant
{
    Q_OBJECT
public:
    BondRealtimeInfo(const QString &windCode);
    virtual ~BondRealtimeInfo();

    //获取股票的实时数据
    //isRealtime = false 获取一次性快照数据
    //isRealtime = true 获取订阅实时数据
    bool requestDataFromServer(const QString &windCode, bool isRealtime = false);
    bool requestDataFromServer(bool isRealtime = false);

    //取消对该只股票实时数据的订阅
    bool cancelRequest();

    QString getBondCode();
    const BondRealtimeData *getBondRealtimeData() const;

    QReadWriteLock *RWLock_realtime;

//protected:
    static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

    //请求号
    WQID                    reqID;
    QString                 bondCode;
    BondRealtimeData        realtimedata;

signals:
    void signal_realtimedata_refresh();

signals:

public slots:
};

#endif // BONDREALTIMEDATA_H

