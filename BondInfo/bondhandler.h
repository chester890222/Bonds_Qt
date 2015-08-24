#ifndef BONDHANDLER_H
#define BONDHANDLER_H

#include <QObject>
#include "basemysql.h"

class BondHandler : public QObject
{
    Q_OBJECT
public:
    explicit BondHandler(QObject *parent = 0);
    void ~BondHandler();

protected:
    void Init();


    BaseMySql *bond_db;


signals:

public slots:
};

#endif // BONDHANDLER_H
