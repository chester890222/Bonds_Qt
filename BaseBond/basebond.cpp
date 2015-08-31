#include "basebond.h"


BaseBond::BaseBond(const QString &code) : BondRealtimeInfo(code) {
//    bondCode = Code;
}

double BaseBond::cal_accInterest() {
    return 0.0;
}

double BaseBond::cal_currentCoupon() {
    return 0.0;
}

double BaseBond::cal_timeToMaturity() {
    return 0.0;
}


void BaseBond::cal_YTMs() {

}
