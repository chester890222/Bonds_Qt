#include "bondpool.h"
#include "BaseWindQuant.h"
#include "bondrealtimeinfo.h"
#include <QDebug>

BondPool BondPool::g_instance;

BondPool::BondPool(QObject *parent) :QObject(parent), BaseWindQuant() {
    isInit = false;
    isReq = false;
    reqID = 0;
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
        //        connect(bondMap[windCodes[i]],SIGNAL(signal_realtimedata_refresh(QString)),this,SLOT(slot_RealtimeDataUpdate(QStringList)));
    }
    //    RWLock_realtime = new QReadWriteLock;
    isInit = true;
    return 0;

}

int BondPool::clear() {
    qDebug()<<Q_FUNC_INFO;

    if (isInit) {
        qDebug() << "isReq=" << isReq;
        if (isReq) {
            cancelRequestFromWind();
        }
        //        delete RWLock_realtime;
        //        RWLock_realtime = NULL;
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

bool BondPool::requestDataFromWind(QString indicators, bool isRealtime) {
    qDebug() << Q_FUNC_INFO;

    bool res = false;
    if (!isInit) {
        qDebug() << "BondPool isInit = false";
        return res;
    }
    if (reqID != 0) {
        qDebug() << "reqID != 0";
        cancelRequestFromWind();//同时会将上次请求的ID清零
    }
    qDebug() << windCodesNumber;
    //    QString code_str = windCodes.join(",");
    QString code_str("019511.SH,019308.SH");
    qDebug() << code_str;

    if (isRealtime) {
        reqID = MyWSQ(code_str, indicators.toStdWString().c_str(), L"REALTIME=Y", dataPro, this);
    } else {
        reqID = MyWSQ(code_str, indicators.toStdWString().c_str(), L"REALTIME=N", dataPro, this);
    }

    if (reqID != 0) {
        res = true;
    }
    qDebug() << Q_FUNC_INFO << "reqID = " << reqID;
    return res;
}

bool BondPool::cancelRequestFromWind() {
    qDebug()<<Q_FUNC_INFO;

    bool res = false;
    if (!isInit) return res;
    if (isReq) {
        res = BaseWindQuant::MyCancelAllRequest();
        isReq = false;
        reqID = 0;
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

int BondPool::dataPro(WQEvent* pEvent, LPVOID pParam) {
    qDebug() << Q_FUNC_INFO;
    if (!pEvent || !pParam) {
        qDebug() << "BondPool dataPro error";
        return 0;
    }

    BondPool *pSri = (BondPool*) pParam;
    QStringList codeList;

    if (pEvent->pQuantData != NULL) {
        //        QWriteLocker locker(pSri->RWLock_realtime);
        int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
        int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
        int timenum = pEvent->pQuantData->ArrDateTime.arrLen;
        qDebug() << "codenum = " << codenum;
        qDebug() << "indnum = " << indnum;
        qDebug() << "timenum = " << timenum;
        VARIANT& data = pEvent->pQuantData->MatrixData;
        if ((data.vt & VT_ARRAY) && (data.parray != NULL)) {
            for (int i = 0; i < codenum;i++) {
                QString tmp_code = pEvent->pQuantData->ArrWindCode.codeArray[i];
                codeList.append(tmp_code);
                qDebug() <<tmp_code;
                QWriteLocker locker(pSri->getBondMap()->value(tmp_code)->RWLock_realtime);
                BondRealtimeData &temp = pSri->getBondMap()->value(tmp_code)->realtimedata;
                temp.windCode = tmp_code;
                qDebug() <<temp.windCode;
                for (int j = 0; j < timenum; j++) {
                    temp.date_db = pEvent->pQuantData->ArrDateTime.timeArray[j];
                    qDebug() << tmp_code << temp.date_db;
                    for (int k = 0; k < indnum; k++) {
                        int pos = i * timenum * indnum + k;

                        //现价，现额，现量
                        if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_LAST") == 0) {
                            temp.rt_last = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_LAST_AMT") == 0) {
                            temp.rt_last_amt = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_LAST_VOL") == 0) {
                            temp.rt_last_vol = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        //买价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BID1") == 0) {
                            temp.rt_bid[0] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BID2") == 0) {
                            temp.rt_bid[1] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BID3") == 0) {
                            temp.rt_bid[2] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BID4") == 0) {
                            temp.rt_bid[3] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BID5") == 0) {
                            temp.rt_bid[4] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        //卖价
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASK1") == 0) {
                            temp.rt_ask[0] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASK2") == 0) {
                            temp.rt_ask[1] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASK3") == 0) {
                            temp.rt_ask[2] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASK4") == 0) {
                            temp.rt_ask[3] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASK5") == 0) {
                            temp.rt_ask[4] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        //买量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BSIZE1") == 0) {
                            temp.rt_bsize[0] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BSIZE2") == 0) {
                            temp.rt_bsize[1] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BSIZE3") == 0) {
                            temp.rt_bsize[2] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BSIZE4") == 0) {
                            temp.rt_bsize[3] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_BSIZE5") == 0) {
                            temp.rt_bsize[4] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        //卖量
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASIZE1") == 0) {
                            temp.rt_asize[0] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASIZE2") == 0) {
                            temp.rt_asize[1] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASIZE3") == 0) {
                            temp.rt_asize[2] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASIZE4") == 0) {
                            temp.rt_asize[3] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[k], "RT_ASIZE5") == 0) {
                            temp.rt_asize[4] = Mytodouble(data.parray->pvData, pos, data.vt);
                        }
                        qDebug() << tmp_code <<temp.rt_last << " " << temp.rt_bid[0];
                    }
                }
            }
        }

    }
    qDebug() << "fine here";
    emit pSri->signal_RealtimeDataUpdate(codeList);
    return 0;
}

