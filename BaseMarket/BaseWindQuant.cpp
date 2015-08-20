#include "BaseWindQuant.h"

bool BaseWindQuant::isAuthorized=false;
bool BaseWindQuant::isMainCallBackRtn=false;

BaseWindQuant::BaseWindQuant()
{
}

BaseWindQuant::~BaseWindQuant()
{
    AuthorizeQuit();
}

int WINAPI BaseWindQuant::CallbackMain(WQEvent* pEvent, LPVOID pParam)
{
    if (!pEvent)
    {
        qDebug()<<QString("CallbackMain:%1").arg(pEvent->ErrCode);
        return -1;
    }

    if(eWQLogin==pEvent->EventType)
    {
        //qDebug()<<"Wind CallBackMain Ok.";
        isMainCallBackRtn = true;

        if( WQERR_OK == pEvent->ErrCode)
        {
            isAuthorized = true;
            qDebug()<<"Wind行情认证成功.";
            //this->signalSendMSG("Wind行情认证成功.");
        }
    }

    return 0;
}

bool BaseWindQuant::WindAuthorize()
{
    if (!isAuthorized)
    {
        //设置主回调函数
        SetEventHandler(CallbackMain);

        isMainCallBackRtn = false;
        WDataAuthorize(NULL);
        while (!isMainCallBackRtn)
        {
            //QThread::msleep(100);
            QCoreApplication::processEvents(QEventLoop::AllEvents, 600);
        }
        isMainCallBackRtn = false;
    }

    return isAuthorized;
}
//bool BaseWindQuant::Authorize(const QString &windAccountId, const QString &windPassword)
//{
//    if (!isAuthorized)
//    {
//        //设置主回调函数
//        SetEventHandler(CallbackMain);

//        WQAUTH_INFO *pAuthInfo = new WQAUTH_INFO();
//        pAuthInfo->bSilentLogin = 1;
//        wcscpy_s(pAuthInfo->strUserName, windAccountId.length() + 1, windAccountId.toStdWString().c_str());
//        wcscpy_s(pAuthInfo->strPassword, windPassword.length() + 1, windPassword.toStdWString().c_str());

//        isMainCallBackRtn = false;
//        WQID err = WDataAuthorize(pAuthInfo);
//        while (!isMainCallBackRtn)
//        {
//            //QThread::msleep(100);
//            QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
//        }
//        isMainCallBackRtn = false;

//        delete pAuthInfo;
//    }

//    return isAuthorized;
//}
bool BaseWindQuant::AuthorizeQuit()
{
    if(isAuthorized)
    {
        if(WQERR_OK==WDataAuthQuit())
        {
            isAuthorized = false;
            qDebug()<<Q_FUNC_INFO<<" OK.";
        }
        else
            return false;
    }

    return true;
}
bool BaseWindQuant::IsAuthorized()
{
    return isAuthorized;
}
bool BaseWindQuant::MyCancelAllRequest()
{
    if (WQERR_OK != CancelAllRequest())
    {
        qDebug()<<Q_FUNC_INFO<<" Error.";
        return false;
    }

    qDebug()<<Q_FUNC_INFO<<" OK.";
    return true;
}

bool BaseWindQuant::MyCancelRequest(WQID rid)
{
    if(rid!=NULL)
    {
        if(WQERR_OK!=CancelRequest(rid))
        {
            qDebug()<<Q_FUNC_INFO<<" Error "<<rid;
            return false;
        }

        rid = NULL;
    }

    qDebug()<<Q_FUNC_INFO<<" OK.";
    return true;
}


//常用wind接口函数
inline LPCWSTR BaseWindQuant::MytoLPCWSTR(QString data)
{
//    //WST error -40522003
//    return data.toStdWString().c_str();

    //from QString to "const wchar_t *" and "LPCWSTR"
    const wchar_t *value=reinterpret_cast<const wchar_t *>(data.utf16());
    return value;

//    //from QString to "WCHAR*" and "LPCWSTR"
//    int leng= sizeof(codes);    //sizeof need "#include <QMetaType>"  //codes.length();
//    WCHAR* temp1;
//    temp1=(WCHAR*)malloc(leng);
//    int asdf= codes.toWCharArray(temp1);
}

WQID BaseWindQuant::MyWSET(LPCWSTR reportName,QString params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSET(reportName,MytoLPCWSTR(params),reqEventHandler,lpReqParam);
}
WQID BaseWindQuant::MyWSET(LPCWSTR reportName,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSET(reportName,params,reqEventHandler,lpReqParam);
}

WQID BaseWindQuant::MyWSS(LPCWSTR windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSS(windcodes,indicators,params,reqEventHandler,lpReqParam);
}
WQID BaseWindQuant::MyWSS(QString windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSS(MytoLPCWSTR(windcodes),indicators,params,reqEventHandler,lpReqParam);
}
WQID BaseWindQuant::MyWSS(QString windcodes, LPCWSTR indicators, QString params, IEventHandler reqEventHandler, LPVOID lpReqParam)
{
    return WSS(MytoLPCWSTR(windcodes),indicators,MytoLPCWSTR(params),reqEventHandler,lpReqParam);
}

WQID BaseWindQuant::MyWSD(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,LPCWSTR endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSD(MytoLPCWSTR(windcodes),indicators,beginTime,endTime,params,reqEventHandler,lpReqParam );
}
WQID BaseWindQuant::MyWSD(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSD(MytoLPCWSTR(windcodes),indicators,beginTime,MytoLPCWSTR(endTime),params,reqEventHandler,lpReqParam );
}
WQID BaseWindQuant::MyWSD(QString windcodes,LPCWSTR indicators,QString beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSD(MytoLPCWSTR(windcodes),indicators,MytoLPCWSTR(beginTime),MytoLPCWSTR(endTime),params,reqEventHandler,lpReqParam );
}

WQID BaseWindQuant::MyWSQ(QString windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam )
{
    return WSQ(MytoLPCWSTR(windcodes),indicators,params,reqEventHandler,lpReqParam);
}
WQID BaseWindQuant::MyWSQ(LPCWSTR windcodes,LPCWSTR indicators,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WSQ(windcodes,indicators,params,reqEventHandler,lpReqParam);
}

WQID BaseWindQuant::MyWST(QString windcodes,LPCWSTR indicators,LPCWSTR beginTime,LPCWSTR endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WST(MytoLPCWSTR(windcodes),indicators,beginTime,endTime,params,reqEventHandler,lpReqParam );
}
WQID BaseWindQuant::MyWST(QString windcodes,LPCWSTR indicators,QString beginTime,QString endTime,LPCWSTR params,IEventHandler reqEventHandler,LPVOID lpReqParam)
{
    return WST(MytoLPCWSTR(windcodes),indicators,MytoLPCWSTR(beginTime),MytoLPCWSTR(endTime),params,reqEventHandler,lpReqParam );
}

//数据类型转换
QString BaseWindQuant::Mytostring(const LPVARIANT data)
{
    QString msg;
    switch(data->vt & VT_BSTR_BLOB)
    {
    case VT_R8:
        msg =QString("1%").arg(data->dblVal);
        break;
    case VT_EMPTY:
        msg = _T("  NaN");
        break;
    case VT_BSTR:
        msg=QString::fromStdWString(data->bstrVal);
        break;
    case VT_DATE:
        msg =QString("1%").arg(data->date);
        break;
    default:
        //msg = "  ";
        //msg += data.bstrVal;
        //msg += "\r\n";
        //msg.Format(_T("   %s"), data->bstrVal);
        break;
    }
    return msg;
}
QString BaseWindQuant::Mytostring(PVOID pData, int pos, VARTYPE vt)
{
    QString msg;
    switch (vt & VT_BSTR_BLOB)
    {
    case VT_VARIANT:
    {
        LPVARIANT data = (LPVARIANT)pData + pos;
        msg = Mytostring(data);
    }
        break;
    case VT_BSTR:
    {
        LPBSTR data = (LPBSTR)pData + pos;
        msg =QString::fromStdWString(*data);
        break;
    }
    case VT_R8:
    {
        DOUBLE * data = (DOUBLE *)pData + pos;
        msg =QString("1%").arg(*data);
        break;
    }
    case VT_DATE:
    {
        DATE * data = (DATE *)pData + pos;
        msg =QString("1%").arg(*data);
        break;
    }
    case VT_I4:
    {
        INT* data = (INT*)pData + pos;
        msg =QString("1%").arg(*data);
        break;
    }
    case VT_I8:
    {
        LONGLONG* data = (LONGLONG*)pData + pos;
        msg =QString("1%").arg(*data);
        break;
    }
    default:
        break;
    }
    return msg;
}

double BaseWindQuant::Mytodouble(const LPVARIANT data)
{
    double value= qQNaN();
    switch(data->vt & VT_BSTR_BLOB)
    {
    case VT_R4:
        value = data->fltVal;
        break;
    case VT_R8:
        value =data->dblVal;
        break;
        //    case VT_DECIMAL:
        //        value = (double)(*data);
        //        break;
    case VT_UI1:
    case VT_I1:
        value = data->bVal;
        break;
    case VT_UI2:
    case VT_I2:
        value = data->iVal;
        break;
    case VT_UI4:
    case VT_I4:
        value = data->lVal;
        break;
    case VT_INT:
        value = data->intVal;
        break;
    case VT_EMPTY:
    case VT_NULL:
        value = 0;
        break;
    default:
        break;
    }
    return value;
}
double BaseWindQuant::Mytodouble(PVOID pData, int pos, VARTYPE vt)
{
    double value= qQNaN();
    switch (vt & VT_BSTR_BLOB)
    {
    case VT_VARIANT:
    {
        LPVARIANT data = (LPVARIANT)pData + pos;
        value = Mytodouble(data);
    }
        break;
    case VT_R8:
    {
        DOUBLE *data = (DOUBLE *)pData + pos;
        value = *data;
        break;
    }
    default:
        break;
    }
    return value;
}

int BaseWindQuant::Mytoint(const LPVARIANT data)
{
    int value= qQNaN();
    switch(data->vt & VT_BSTR_BLOB)
    {
    case VT_I4:
        value =data->intVal;
        break;
    case VT_I2:
        value =data->iVal;
        break;
    case VT_DATE:
        value =data->date;
        break;
    case VT_EMPTY:

        break;
    default:
        break;
    }
    return value;
}
int BaseWindQuant::Mytoint(PVOID pData, int pos, VARTYPE vt)
{
    int value = qQNaN();
    switch (vt & VT_BSTR_BLOB)
    {
    case VT_VARIANT:
    {
        LPVARIANT data = (LPVARIANT)pData + pos;
        value = Mytoint(data);
    }
        break;
    case VT_I4:
    {
        INT* data = (INT*)pData + pos;
        value = *data;
        break;
    }
    default:
        break;
    }
    return value;
}

