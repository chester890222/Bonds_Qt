#include "bondpool.h"


BondPool *BondPool::getInstance() {
    return &g_instance;
}

BondPool::BondPool() {
    isInit = false;
    isReq = false;
    windCodesSize = 0;
}

BondPool::~BondPool() {
    clear();
}

int BondPool::init(const QStringList windCodesList) {
    if (isInit) {
        clear();
    }

    windCodesSize = windCodes.size();
    for (int i=0; i < this->windCodesSize; i++) {
        BondRealtimeInfo *realtimeInfoTemp = new BondRealtimeInfo(windCodesList[i]);
        windCodes.append(windCodesList[i]);
//####################################
    }




}
