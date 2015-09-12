#include "bondpool.h"
#include "BaseWindQuant.h"
#include <QDebug>

BondPool BondPool::g_instance;

BondPool::BondPool(QObject *parent) : QObject(parent) {
    isInit = false;
    isReq = false;
    windCodesNumber = 0;
}

BondPool::~BondPool() {
    clear();
}

int BondPool::init(const QStringList &windCodesList) {
//    qDebug() << Q_FUNC_INFO;
    if (isInit) {
        clear();
    }

    windCodesNumber = windCodesList.size();
    for (int i=0; i < this->windCodesNumber; i++) {
        BaseBond *baseBondTemp = new BaseBond(windCodesList.at(i));
        windCodes.append(windCodesList.at(i));
        bondMap.insert(windCodesList.at(i), baseBondTemp);
        connect(bondMap[windCodes[i]],SIGNAL(signal_realtimedata_refresh(QString)),this,SLOT(slot_RealtimeDataUpdate(QString)));
    }

    isInit = true;
    return 0;

}

int BondPool::clear() {
    qDebug()<<Q_FUNC_INFO;

    if (isInit) {
        if (isReq) {
            cancelRequestFromWind();
        }
        this->windCodesNumber = 0;
//        this->windCodes.clear();
        qDebug() << "fine up here1";
        qDeleteAll(bondMap.begin(),bondMap.end());
        qDebug() << "fine up here2";
        bondMap.clear();
    }

    isInit = false;
    qDebug()<<Q_FUNC_INFO <<"DONE!";
    return 0;
}

bool BondPool::requestDataFromWind() {
//########################################
    bool res = true;
    if (!isInit) return false;
    for (int i =0; i< windCodesNumber; i++) {
        res = res && bondMap[windCodes[i]]->requestDataFromWind(true);
    }

    if (res == false) {
        cancelRequestFromWind();
        qDebug() << Q_FUNC_INFO << "failure!!!";
    } else {
        isReq = true;
    }

    return res;
}

bool BondPool::cancelRequestFromWind() {
    qDebug()<<Q_FUNC_INFO;

    bool res = false;
    if (!isInit) return res;
    if (isReq) {
        res = BaseWindQuant::MyCancelAllRequest();
        isReq = false;
    }
    qDebug()<<Q_FUNC_INFO <<"DONE!";

    return res;
}


BondPool *BondPool::getInstance() {
    return &g_instance;
}

const QStringList *BondPool::getWindCodes() {
    return &this->windCodes;
}

int BondPool::getwindCodesNumber() {
    return this->windCodesNumber;
}

const QMap<QString, BaseBond*> *BondPool::getBondMap() {
    return &this->bondMap;
}

void BondPool::slot_RealtimeDataUpdate(QString code) {
    emit signal_RealtimeDataUpdate();
}
