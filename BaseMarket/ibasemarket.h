#ifndef IBASEMARKET
#define IBASEMARKET

#include <QObject>
#include <QStringList>


class IBaseMarket:public QObject
{
    Q_OBJECT
public:
    IBaseMarket(){}
    virtual ~IBaseMarket(){}


signals:
    void signalSendMSG(QString msg,int type=2);
    void signalAuthorizeFinished(bool rtn);
    void signalPriceChanged(QStringList codesRefresh,QStringList fieldsRefresh = QStringList(),int tag=0);


public slots:
    virtual void Authorize()=0;
    virtual void quit()=0;
};



#endif // IBASEMARKET

