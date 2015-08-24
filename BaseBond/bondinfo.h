#ifndef BONDINFO_H
#define BONDINFO_H

#include <QObject>
#include <QMap>
#include <QDate>
#include "bondrealtimeinfo.h"




class BondInfo : public QObject
{
    Q_OBJECT
public:
    explicit BondInfo(QObject *parent = 0);
    BondInfo(QString windcode);
    Bond_db_info* getBondInfo();
    BondRealTimeInfo* getRealtimeInfo();


protected:
    QString code;
    double timeToMaturity;
    double accrInterest;

    Bond_db_info bondInfo;
    BondRealTimeInfo realtimeInfo;



signals:

public slots:
};

#endif // BONDINFO_H
