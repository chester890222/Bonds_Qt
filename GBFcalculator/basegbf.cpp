#include "basegbf.h"

BaseGBF::BaseGBF(const QString &code) : BondRealtimeInfo(code)
{

}

QDate BaseGBF::getDate(QString type) {
    if (type == "List") {
        return listDate;
    } else if (type == "Off List") {
        return offListDate;
    } else if (type == "First Delivery") {
        return firstDeliveryDate;
    } else if (type == "Last Delivery") {
        return lastDeliveryDate; //最后交券日
    } else if (type == "Last Match") {
        return lastMatchDate;
    } else if (type == "Last Receive") {
        return lastReceiveDate; //最后收券日
    } else {
        qDebug() << "Not Recognized Date!!!";
        return QDate(1900,1,1);
    }
}

void setDate(QString type, QDate date);
QMap<QString, double> getConvFactors();
QMap<QString, BaseBond*>* getDeliBonds();
