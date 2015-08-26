#ifndef BONDPOOL_H
#define BONDPOOL_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "bondrealtimeinfo.h"

//债券池，单例模式
class BondPool
{
public:
    static BondPool *getInstance();
    int init(const QStringList windCodesList);
    int clear();

    bool requestDataFromServer();
    bool cancelRequestFromServer();

    const QStringList *getWindCodes();
    const QMap<QString, BondRealtimeInfo*> *getBond_Info_map;



protected:
    BondPool();
    ~BondPool();

    bool isInit;
    bool isReq;

    int windCodesSize;
    QStringList windCodes;
    QMap<QString, BondRealtimeInfo*> bond_info_map;

private:
    static BondPool g_instance;



};

#endif // BONDPOOL_H
