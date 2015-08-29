#include "bondpool.h"
#include "BaseWindQuant.h"

BondPool::BondPool() {
    isInit = false;
    isReq = false;
    windCodesNumber = 0;
}

BondPool::~BondPool() {
    clear();
}

int BondPool::init(const QStringList windCodesList) {
    if (isInit) {
        clear();
    }

    windCodesNumber = windCodes.Number();
    for (int i=0; i < this->windCodesNumber; i++) {
        BondRealtimeInfo *realtimeInfoTemp = new BondRealtimeInfo(windCodesList[i]);
        windCodes.append(windCodesList[i]);
        bondMap.insert(windCodesList[i], realtimeInfoTemp);
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

        qDeleteAll(bond_info_map);
        bond_info_map.clear();


    }

    isInit = false;
    return 0;
}

bool BondPool::requestDataFromWind() {
//########################################
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


static BondPool *BondPool::getInstance() {
    return &this->g_instance;
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


