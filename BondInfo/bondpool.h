#ifndef BONDPOOL_H
#define BONDPOOL_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "basebond.h"

//债券池，单例模式
class BondPool
{
public:
    /////////////////////////////////////////////////
    int getwindCodesNumber();
    static BondPool *getInstance();
    const QStringList *getWindCodes();
    const QMap<QString, BaseBond*> *getBondMap();
    /////////////////////////////////////////////////

    BondPool();
    ~BondPool();

    int init(const QStringList &windCodesList);
    int clear();

    bool requestDataFromWind();
    bool cancelRequestFromWind();



//protected:
    bool isInit;
    bool isReq;

    int windCodesNumber;
    QStringList windCodes;

    QMap<QString, BaseBond*> bondMap;

//private:
    static BondPool g_instance;



};

#endif // BONDPOOL_H
