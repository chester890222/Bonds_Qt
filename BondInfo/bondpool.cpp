#include "bondpool.h"
#include "BaseWindQuant.h"
#include <QDebug>

BondPool BondPool::g_instance;


BondPool::BondPool() {
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
    }

    isInit = true;
    return 0;

}

int BondPool::clear() {
    if (isInit) {
        if (isReq) {
            cancelRequestFromWind();
        }
        this->windCodesNumber = 0;
        this->windCodes.clear();

        qDeleteAll(bondMap);
        bondMap.clear();
    }

    isInit = false;
    return 0;
}

bool BondPool::requestDataFromWind() {
//########################################
    if (!isInit) return false;

    bool res = false;
    return res;
}

bool BondPool::cancelRequestFromWind() {
    bool res = false;
    if (!isInit) return res;
    if (isReq) {
        res = BaseWindQuant::MyCancelAllRequest();
        isReq = false;
    }
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

