#include "bondrealtimeinfo.h"
#include "math.h"
#include <QDebug>
#include <QThread>


BondRealtimeInfo::BondRealtimeInfo(const QString &windCode) {
    this->windCode = windCode;
    this->reqID = 0;
    RWLock_realtime = new QReadWriteLock;
//    connect(this,SIGNAL(signal_realtimedata_refresh()),this,SLOT(slot_update_histdata()),Qt::DirectConnection);
}

BondRealtimeInfo::~BondRealtimeInfo() {
//    qDebug() << "BondRealtimeInfo::~BondRealtimeInfo is called on thread " << QThread::currentThreadId();
    if (RWLock_realtime != NULL) {
        delete RWLock_realtime;
        RWLock_realtime = NULL;
    }

    cancelRequest();
}


QString BondRealtimeInfo::getBondCode() {
//    qDebug() << Q_FUNC_INFO();
    return this->windCode;
}

const BondRealtimeData* BondRealtimeInfo::getBondRealtimeData() const{
//    qDebug() << Q_FUNC_INFO();
    QReadLocker locker(RWLock_realtime);
    const BondRealtimeData* temp = &this->realtimedata;
    return temp;
}

const BaseBond* BondRealtimeInfo::getBaseBond() const{
    QReadLocker locker(RWLock_baseBond);
    const BaseBond* temp = &this->baseBond;
    return temp;
}

////////////////////////////////////////////////////////////

bool BondRealtimeInfo::requestDataFromServer(const QString &windCode, bool isRealtime) {
//    qDebug() << Q_FUNC_INFO();

    this->windCode = windCode;

    if (reqID != 0) {
        cancelRequest();//同时会将上次请求的ID清零
    }

    std::wstring indicators = L"rt_vwap,rt_high,rt_low,rt_last,rt_last_amt,rt_last_vol,rt_latest,rt_vol,rt_amt,\
            rt_bid1,rt_bid2,rt_bid3,rt_bid4,rt_bid5,\
            rt_ask1,rt_ask2,rt_ask3,rt_ask4,rt_ask5,\
            rt_bsize1,rt_bsize2,rt_bsize3,rt_bsize4,rt_bsize5,\
            rt_asize1,rt_asize2,rt_asize3,rt_asize4,rt_asize5";

    if (isRealtime) {
        reqID = MyWSQ(this->windCode, indicators.c_str(), L"REALTIME=Y", dataPro, this);
    } else {
        reqID = MyWSQ(this->windCode, indicators.c_str(), L"REALTIME=N", dataPro, this);
    }

    bool res = false;
    if (reqID != 0) {
//        qDebug() << Q_FUNC_INFO();
        res = true;
    }
    return res;
}

bool BondRealtimeInfo::requestDataFromServer(bool isRealtime) {
//    qDebug() << Q_FUNC_INFO();
    return requestDataFromServer(this->windCode, isRealtime);
}

bool BondRealtimeInfo::cancelRequest() {

    bool res = false;
    if (reqID != 0) {
        if (WQERR_OK == MyCancelRequest(reqID)) {
            res = true;
        }
        reqID = 0; //请求号清零
    }
    return res;
}

void BondRealtimeInfo::setBond_db_info(BondType bType, QString code, QString name, InterestType iType, double faceValue, QMap<QDate, double> coupons, double paymentFrequency, QDate carryDate, QDate listDate, QDate offlistDate, QDate MaturityDate, double IssueAmount) {
    this->bond_db_info.bType = bType;
    this->bond_db_info.code = code;
    this->bond_db_info.name = name;
    this->bond_db_info.iType = iType;
    this->bond_db_info.faceValue = faceValue;
    this->bond_db_info.coupons = coupons;
    this->bond_db_info.paymentFrequency = paymentFrequency;
    this->bond_db_info.carryDate = carryDate;
    this->bond_db_info.listDate = listDate;
    this->bond_db_info.offlistDate = offlistDate;
    this->bond_db_info.MaturityDate = MaturityDate;
    this->bond_db_info.IssueAmount = IssueAmount;
}


int BondRealtimeInfo::dataPro(WQEvent* pEvent, LPVOID pParam) {
    if (!pEvent || !pParam) {
        return 0;
    }

    BondRealtimeInfo *pSri = (BondRealtimeInfo*) pParam;

    if (pEvent->pQuantData != NULL) {
        QWriteLocker locker(pSri->RWLock_realtime);
        int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
        int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
        int timenum = pEvent->pQuantData->ArrDateTime.arrLen;

        VARIANT& data = pEvent->pQuantData->MatrixData;
        if ((data.vt & VT_ARRAY) && (data.parray != NULL)) {
            if (timenum == 1) {
                if (codenum == 1) {

                    BondRealtimeData &temp = pSri->realtimedata;
                    temp.date_db = pEvent->pQuantData->ArrDateTime.timeArray[0];
                    temp.windCode = pSri->windCode;
                    //C++ switch 不支持字符串case
                    for (int i = 0; i < indnum; i++) {
                        //最高最低价，均价
                        if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_HIGH") == 0) {
                            temp.rt_high = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LOW") == 0) {
                            temp.rt_low = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_VWAP") == 0) {
                            temp.rt_vwap = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //现价，现额，现量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST") == 0) {
                            temp.rt_last = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_AMT") == 0) {
                            temp.rt_last_amt = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_VOL") == 0) {
                            temp.rt_last_vol = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //最新成交价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LATEST") == 0) {
                            temp.rt_latest = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //成交量，成交额
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_VOL") == 0) {
                            temp.rt_vol = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_AMT") == 0) {
                            temp.rt_amt = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //买价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID1") == 0) {
                            temp.rt_bid[0] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID2") == 0) {
                            temp.rt_bid[1] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID3") == 0) {
                            temp.rt_bid[2] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID4") == 0) {
                            temp.rt_bid[3] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID5") == 0) {
                            temp.rt_bid[4] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //卖价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK1") == 0) {
                            temp.rt_ask[0] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK2") == 0) {
                            temp.rt_ask[1] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK3") == 0) {
                            temp.rt_ask[2] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK4") == 0) {
                            temp.rt_ask[3] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK5") == 0) {
                            temp.rt_ask[4] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //买量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE1") == 0) {
                            temp.rt_bsize[0] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE2") == 0) {
                            temp.rt_bsize[1] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE3") == 0) {
                            temp.rt_bsize[2] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE4") == 0) {
                            temp.rt_bsize[3] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE5") == 0) {
                            temp.rt_bsize[4] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        //卖量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE1") == 0) {
                            temp.rt_asize[0] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE2") == 0) {
                            temp.rt_asize[1] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE3") == 0) {
                            temp.rt_asize[2] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE4") == 0) {
                            temp.rt_asize[3] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE5") == 0) {
                            temp.rt_asize[4] = Mytodouble(data.parray->pvData, i, data.vt);
                        }
                    }
                }
            }
        }
    }
    emit pSri->signal_realtimedata_refresh();

    return 0;
}







