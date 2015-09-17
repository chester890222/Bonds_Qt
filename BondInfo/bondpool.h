#ifndef BONDPOOL_H
#define BONDPOOL_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QReadWriteLock>

#include "basebond.h"
#include "BaseWindQuant.h"

//债券池，单例模式
class BondPool : public QObject, public BaseWindQuant
{
    Q_OBJECT

public:
    /////////////////////////////////////////////////
    int getwindCodesNumber();
    static BondPool *getInstance();
    const QStringList *getWindCodes();
    const QMap<QString, BaseBond*> *getBondMap();
    /////////////////////////////////////////////////

    explicit BondPool(QObject *parent=0);
    ~BondPool();

    int init(const QStringList &windCodesList);
    int clear();

    bool requestDataFromWind(QString indicators, bool isRealtime = false);
    bool cancelRequestFromWind();

    static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);


//protected:
    bool isInit;
    bool isReq;
    WQID reqID;

    int windCodesNumber;
    QStringList windCodes;

    QMap<QString, BaseBond*> *bondMap;

//private:
    static BondPool *g_instance;

signals:
    void signal_RealtimeDataUpdate(QStringList);

public slots:

};

#endif // BONDPOOL_H
