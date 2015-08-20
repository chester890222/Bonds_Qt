#include "BaseTDF.h"


BaseTDF::BaseTDF()
    :g_hTDF(NULL)
{
    gCodeSHTDF.insert("000001.SH","999999.SH");
    gCodeSHTDF.insert("000002.SH","999998.SH");
    gCodeSHTDF.insert("000003.SH","999997.SH");
    gCodeSHTDF.insert("000004.SH","999996.SH");
    gCodeSHTDF.insert("000005.SH","999995.SH");
    gCodeSHTDF.insert("000006.SH","999994.SH");
    gCodeSHTDF.insert("000007.SH","999993.SH");
    gCodeSHTDF.insert("000008.SH","999992.SH");

    gCodeSHTDF.insert("000010.SH","999991.SH");
    gCodeSHTDF.insert("000011.SH","999990.SH");
    gCodeSHTDF.insert("000012.SH","999989.SH");
    gCodeSHTDF.insert("000013.SH","999988.SH");

    gCodeSHTDF.insert("000016.SH","999987.SH");
    gCodeSHTDF.insert("000015.SH","999986.SH");
    gCodeSHTDF.insert("000300.SH","000300.SH");
}

BaseTDF::~BaseTDF()
{
    TDFQuit();
    qDebug()<<Q_FUNC_INFO;
}

QVariant getProperty(QString key)
{
    QString fileName = "configMarket.ini";
    QSettings *settings = new QSettings(fileName,QSettings::IniFormat);
    return settings->value(key);
}

void BaseTDF::Set_Env()
{
    //开始连接时候超时时间，秒数（如果为0，则设为1）
    TDF_SetEnv(TDF_ENVIRON_OPEN_TIME_OUT, getProperty("TDF/OpenTimeOut").toInt());
    //心跳间隔，秒数（如果为0，则设为1）
    TDF_SetEnv(TDF_ENVIRON_HEART_BEAT_INTERVAL, getProperty("TDF/HeartBeatGap").toInt());
    //丢失心跳次数视作掉线
    TDF_SetEnv(TDF_ENVIRON_MISSED_BEART_COUNT, getProperty("TDF/MissHeartBeatCount").toInt());
    //打开log
    TDF_SetEnv(TDF_ENVIRON_OUT_LOG, 1);
    TDF_SetEnv(TDF_ENVIRON_NOTTRANS_OLD_DATA, 1);
}
void BaseTDF::Set_OPEN_SETTING(TDF_OPEN_SETTING& open_settings)
{
    //TDF_OPEN_SETTING open_settings = {0};
    memset(&open_settings, 0, sizeof(open_settings));

    strncpy(open_settings.szIp, getProperty("TDF/IP").toByteArray().data(), sizeof(open_settings.szIp)-1);
    _snprintf(open_settings.szPort, sizeof(open_settings.szPort)-1, "%d", getProperty("TDF/Port").toInt());
    strncpy(open_settings.szUser, getProperty("TDF/User").toByteArray().data(), sizeof(open_settings.szUser)-1);
    strncpy(open_settings.szPwd, getProperty("TDF/Pwd").toByteArray().data(), sizeof(open_settings.szPwd)-1);

    //格式YYMMDD，例如20130101；0表示最新行情
    open_settings.nDate = 0;
    //格式HHMMSS，例如94500表示9点45分；-1表示从头传输0xffffffff，0表示最新行情
    int startTime = getProperty("TDF/StartTime").toInt();
    if(-1==startTime)
        open_settings.nTime = 0xffffffff;
    else
        open_settings.nTime = startTime;

    //订阅的股票
    //订阅列表，对于已知的国内6所(sh;sz;cf;shf;czc;dce)代码不用区分大小写，未来加入的交易所则必须严格大小写。
    //代码之间用英文字符 ; 分割。如果为空，则订阅全部代码
    //代码格式如：000001.sz;600000.sh;510300.sh;159919.SZ;150171.sz;150172.sz
    open_settings.szSubScriptions = "000300.SH";
    //订阅的市场
    //市场列表，不用区分大小写，用英文字符 ; 分割；如果为空，则订阅全部市场。sh;sz;cf;shf;czc;dce;
    open_settings.szMarkets = "sh;sz";


    //reconnect count, default:99999999
    open_settings.nReconnectCount = getProperty("TDF/ReconnectCount").toInt();
    //reconnect gap, unit: second, default:5 seconds, range(1-3600)
    open_settings.nReconnectGap = getProperty("TDF/nReconnectGap").toInt();
    //data type:INDEX;TRANSACTION;ORDER;ORDERQUEUE;, if null then all data are subscribed.
    open_settings.nTypeFlags = DATA_TYPE_INDEX | DATA_TYPE_TRANSACTION;//|DATA_TYPE_TRANSACTION|DATA_TYPE_ORDER|DATA_TYPE_ORDERQUEUE;

    open_settings.nProtocol = 0;//协议号，为0则为默认，或者0x6001
}
void BaseTDF::Set_PROXY_SETTING(TDF_PROXY_SETTING& proxy_settings)
{
    memset(&proxy_settings,0,sizeof(proxy_settings));

    QString proxyType = getProperty("TDF/ProxyType").toString();
    if(proxyType == "SOCK4A")
        proxy_settings.nProxyType = TDF_PROXY_SOCK4;
    else if(proxyType == "SOCK4A")
        proxy_settings.nProxyType = TDF_PROXY_SOCK4A;
    else if(proxyType == "SOCK5")
        proxy_settings.nProxyType = TDF_PROXY_SOCK5;
    else if(proxyType == "HTTP11")
        proxy_settings.nProxyType = TDF_PROXY_HTTP11;

    strncpy(proxy_settings.szProxyHostIp, getProperty("TDF/ProxyHostIP").toByteArray().data(), sizeof(proxy_settings.szProxyHostIp)-1);
    _snprintf(proxy_settings.szProxyPort, sizeof(proxy_settings.szProxyPort)-1, "%d", getProperty("TDF/ProxyHostPort").toInt());
    strncpy(proxy_settings.szProxyUser, getProperty("TDF/ProxyUser").toByteArray().data(), sizeof(proxy_settings.szProxyUser)-1);
    strncpy(proxy_settings.szProxyPwd, getProperty("TDF/ProxyPassword").toByteArray().data(), sizeof(proxy_settings.szProxyPwd)-1);
}

void BaseTDF::TDFAuthorize(TDF_SystemMsgHandler sysCallBack,TDF_DataMsgHandler dataCallBack)
{
    qDebug()<<Q_FUNC_INFO<<QThread::currentThreadId();

    Set_Env();
    Set_OPEN_SETTING(open_settings);
    open_settings.pfnSysMsgNotify =sysCallBack;
    open_settings.pfnMsgHandler = dataCallBack ;

    TDF_ERR nErr = TDF_ERR_SUCCESS;
    bool isEnableProxy = getProperty("TDF/EnableProxy").toBool();
    if(isEnableProxy)
    {
        TDF_PROXY_SETTING proxy_settings = {(TDF_PROXY_TYPE)0};
        Set_PROXY_SETTING(proxy_settings);
        g_hTDF = TDF_OpenProxy(&open_settings, &proxy_settings, &nErr);
    }
    else
    {
        g_hTDF = TDF_Open(&open_settings, &nErr);
    }

    //    delete[] open_settings.szMarkets;
    //    delete[] open_settings.szSubScriptions;

    //AddSubscriptions("000001.sz;600000.sh;510300.sh;159919.SZ;150171.sz;150172.sz");
}
void BaseTDF::TDFQuit()
{
    if(g_hTDF!=NULL)
        TDF_Close(g_hTDF);
}



void BaseTDF::SetSubscriptions(QString codes)
{
    TDF_SetSubscription(g_hTDF, codes.toStdString().c_str(), SUBSCRIPTION_SET);
}
void BaseTDF::AddSubscriptions(QStringList codeList)
{
    QStringList codesList;
    foreach(QString code,codeList)
        codesList.append(CodeShToTdf(code));

    TDF_SetSubscription(g_hTDF, codesList.join(";").toStdString().c_str(), SUBSCRIPTION_ADD);
}
void BaseTDF::AddSubscriptions(QString code)
{
    TDF_SetSubscription(g_hTDF, CodeShToTdf(code).toStdString().c_str(), SUBSCRIPTION_ADD);
}
void BaseTDF::DelSubscriptions(QString codes)
{
    TDF_SetSubscription(g_hTDF, codes.toStdString().c_str(), SUBSCRIPTION_DEL);
}
