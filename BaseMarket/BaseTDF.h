#ifndef BASETDF_H
#define BASETDF_H


#include <QDebug>
#include <assert.h>
#include <QDateTime>
#include "time.h"
#include "tdf/include/TDFAPI.h"
#include "tdf/include/TDFAPIStruct.h"
#include "tdf/include/TDFAPIVersion.h"
#include <QThread>
#include <QSettings>


class BaseTDF
{
public:
    BaseTDF();
    virtual ~BaseTDF();

    void TDFAuthorize(TDF_SystemMsgHandler sysCallBack,TDF_DataMsgHandler dataCallBack);
    void TDFQuit();


    void SetSubscriptions(QString codes);
    void AddSubscriptions(QStringList codes);
    void AddSubscriptions(QString code);
    void DelSubscriptions(QString codes);
    //void ClearSubscriptions(QString codes);


protected:
    bool isNeedTdfCode(QString& code)
    {
        return code.contains(".SH",Qt::CaseInsensitive) && code.startsWith("99");
    }
    QString CodeShToTdf(QString& shCode)
    {
        if(gCodeSHTDF.contains(shCode))
            return gCodeSHTDF.value(shCode);
        else if(isNeedShCode(shCode))
            return "99"+shCode.mid(2);
        else
            return shCode;
    }
    QString CodeTdfToSh(QString& tdfCode)
    {
        if(gCodeSHTDF.values().contains(tdfCode))
            return  gCodeSHTDF.key(tdfCode);
        else if(isNeedTdfCode(tdfCode))
            return "00"+tdfCode.mid(2);
        else
            return tdfCode;
    }

private:
    QHash<QString,QString> gCodeSHTDF;
    bool isNeedShCode(QString& code)
    {
        return code.contains(".SH",Qt::CaseInsensitive) && code.startsWith("00");
    }



private:
    THANDLE g_hTDF;
    TDF_OPEN_SETTING open_settings;


    void Set_Env();//设置环境变量 TDF_SetEnv
    void Set_OPEN_SETTING(TDF_OPEN_SETTING& open_settings);//设置连接参数 TDF_OPEN_SETTING
    void Set_PROXY_SETTING(TDF_PROXY_SETTING& proxy_settings);//设置proxy_settings
};

#endif // BASETDF_H
