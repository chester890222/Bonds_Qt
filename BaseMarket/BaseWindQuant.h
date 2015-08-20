#ifndef BASEWINDQUANT_H
#define BASEWINDQUANT_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include "Wind/Quant/WindQuantAPI.h"


class BaseWindQuant
{
public:
    BaseWindQuant();
    virtual ~BaseWindQuant();

    static bool WindAuthorize();
    //static bool Authorize(const QString &windAccountId, const QString &windPassword);
    static bool AuthorizeQuit();
    static bool IsAuthorized();

    static bool MyCancelAllRequest();
    static bool MyCancelRequest(WQID rid);

private:
    static int WINAPI CallbackMain(WQEvent* pEvent, LPVOID pParam);


public://option修改之后，改为protected:
    static QString Mytostring(const LPVARIANT data);
    static QString Mytostring(PVOID pData, int pos, VARTYPE vt);
    static double Mytodouble(const LPVARIANT data);
    static double Mytodouble(PVOID pData, int pos, VARTYPE vt);
    static int Mytoint(const LPVARIANT data);
    static int Mytoint(PVOID pData, int pos, VARTYPE vt);

    static inline LPCWSTR MytoLPCWSTR(QString data);

public://option修改之后，改为protected:
    static WQID MyWSET(LPCWSTR reportName,QString params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWSET(LPCWSTR reportName,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);

    static WQID MyWSS(LPCWSTR windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWSS(QString windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWSS(QString windcodes,LPCWSTR indicators,QString params,IEventHandler reqEventHandler,LPVOID lpReqParam);

    static WQID MyWSD(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,LPCWSTR endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWSD(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWSD(QString windcodes,LPCWSTR indicators,QString beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);

    static WQID MyWSQ(LPCWSTR windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam );
    static WQID MyWSQ(QString windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam );

    static WQID MyWST(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,LPCWSTR endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);
    static WQID MyWST(QString windcodes,LPCWSTR indicators,QString beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam);

protected:
    static bool isAuthorized;
    static bool isMainCallBackRtn;
};

#endif // BASEWINDQUANT_H
