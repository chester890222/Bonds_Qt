#include "basebond.h"


BaseBond::BaseBond(QString windCode) {
    bondCode = windCode;
    bondRealtimeInfo = BondRealtimeInfo(windCode);
}

double BaseBond::cal_accInterest() {

}

double BaseBond::cal_currentCoupon() {

}

double BaseBond::cal_timeToMaturity() {

}


void BaseBond::cal_YTMs() {

}
