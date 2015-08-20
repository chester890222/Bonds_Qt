#include "bondrealtimeinfo.h"
#include "math.h"
#include <QDebug>
#include <QThread>

BondRealTimeInfo::BondRealTimeInfo(QObject *parent):BaseWindQuant(parent)
{
}


BondRealTimeInfo::BondRealTimeInfo(const QString &windCode) {
    this->windCode = windCode;
    this->reqID = 0;
//    this->isWSTqryDone = false;
    RWLock_realtime = new QReadWriteLock;
//    connect(this,SIGNAL(signal_realtimedata_refresh()),this,SLOT(slot_update_histdata()),Qt::DirectConnection);
}

BondRealTimeInfo::~BondRealTimeInfo() {
//    qDebug() << "BondRealTimeInfo::~BondRealTimeInfo is called on thread " << QThread::currentThreadId();
    if (RWLock_realtime != NULL) {
        delete RWLock_realtime;
        RWLock_realtime = NULL;
    }

    cancelRequest();
}


QString BondRealTimeInfo::getBondCode() {
//    qDebug() << Q_FUNC_INFO();
    return this->windCode;
}

const BondRealTimeInfo* BondRealTimeInfo::getBondRealtimeData() const{
//    qDebug() << Q_FUNC_INFO();
    QReadLocker locker(RWLock_realtime);
    const BondRealtimeData* temp = &this->realtimedata;
    return temp;
}

const Bond_db_info* BondRealTimeInfo::getBond_db_info() const{
    QReadLocker locker(RWLock_bond_db);
    const Bond_db_info* temp = &this->bondinfo;
    return temp;
}

////////////////////////////////////////////////////////////

bool BondRealTimeInfo::requestDataFromServer(const QString &windCode, bool isRealtime = false) {
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

bool BondRealTimeInfo::requestDataFromServer(bool isRealtime) {
//    qDebug() << Q_FUNC_INFO();
    return requestDataFromServer(this->windCode, isRealtime);
}

bool BondRealTimeInfo::cancelRequest() {

    bool res = false;
    if (reqID != 0) {
        if (WQERR_OK == MyCancelRequest(reqID)) {
            res = true;
        }
        reqID = 0; //请求号清零
    }
    return res;
}

int BondRealTimeInfo::dataPro(WQEvent* pEvent, LPVOID pParam) {
    if (!pEvent || !pParam) {
        return 0;
    }

    BondRealTimeInfo *pSri = (BondRealTimeInfo*) pParam;

    if (pEvent->pQuantData != NULL) {
        QWriteLocker locker(pSri->RWLock_realtime);
        int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
        int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
        int timenum = pEvent->pQuantData->ArrDateTime.arrLen;

        VARIANT& data = pEvent->pQuantData->MatrixData;
        if ((data.vt & VT_ARRAY) && (data.parray != NULL)) {
            if (timenum == 1) {
                if (codenum == 1) {

                    BondRealTimeInfo &temp = pSri->realtimedata;
                    temp.date_db = pEvent->pQuantData->ArrDateTime.timeArray[0];
                    temp.windCode = pSri->windCode;
                    //C++ switch 不支持字符串case
                    for (int i = 0; i < indnum; i++) {
                        //最高最低价，均价
                        if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_HIGH") == 0) {
                            temp.rt_high = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LOW") == 0) {
                            temp.rt_low = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_VWAP") == 0) {
                            temp.rt_vwap = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //现价，现额，现量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST") == 0) {
                            temp.rt_last = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_AMT") == 0) {
                            temp.rt_last_amt = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_VOL") == 0) {
                            temp.rt_last_vol = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //最新成交价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LATEST") == 0) {
                            temp.rt_latest = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //成交量，成交额
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_VOL") == 0) {
                            temp.rt_vol = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_AMT") == 0) {
                            temp.rt_amt = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //买价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID1") == 0) {
                            temp.rt_bid[0] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID2") == 0) {
                            temp.rt_bid[1] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID3") == 0) {
                            temp.rt_bid[2] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID4") == 0) {
                            temp.rt_bid[3] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID5") == 0) {
                            temp.rt_bid[4] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //卖价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK1") == 0) {
                            temp.rt_ask[0] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK2") == 0) {
                            temp.rt_ask[1] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK3") == 0) {
                            temp.rt_ask[2] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK4") == 0) {
                            temp.rt_ask[3] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK5") == 0) {
                            temp.rt_ask[4] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //买量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE1") == 0) {
                            temp.rt_bsize[0] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE2") == 0) {
                            temp.rt_bsize[1] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE3") == 0) {
                            temp.rt_bsize[2] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE4") == 0) {
                            temp.rt_bsize[3] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE5") == 0) {
                            temp.rt_bsize[4] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        //卖量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE1") == 0) {
                            temp.rt_asize[0] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE2") == 0) {
                            temp.rt_asize[1] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE3") == 0) {
                            temp.rt_asize[2] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE4") == 0) {
                            temp.rt_asize[3] = toDouble(data.parray->pvData, i, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE5") == 0) {
                            temp.rt_asize[4] = toDouble(data.parray->pvData, i, data.vt);
                        }
                    }
                }
            }
        }
    }
    emit pSri->signal_realtimedata_refresh();

    return 0;
}







