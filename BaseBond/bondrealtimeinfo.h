#ifndef BONDREALTIMEDATA_H
#define BONDREALTIMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>
#include <QDateTime>
#include <QReadWriteLock>
#include "BaseWindQuant.h"
#include "basebond.h"



class BondRealtimeInfo : public QObject, BaseWindQuant
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
    const BaseBond     *getBaseBond() const;

    QReadWriteLock *RWLock_realtime;
    QReadWriteLock *RWLock_baseBond;

    void setBaseBond(BondType bType, QString code, QString name, InterestType iType,
                         double faceValue, QMap<QDate, double> coupons, double paymentFrequency,
                         QDate carryDate, QDate listDate, QDate offlistDate, QDate MaturityDate, double IssueAmount);

protected:
    static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

    //请求号
    WQID                    reqID;
    QString                 windCode;
    BondRealtimeData        realtimedata;
    BaseBond                baseBond;

signals:
    void signal_realtimedata_refresh();

signals:

public slots:
};

#endif // BONDREALTIMEDATA_H

