#ifndef BONDHANDLER_H
#define BONDHANDLER_H

#include <QObject>
#include "basemysql.h"
#include "BaseWindQuant.h"

class BondHandler : public QObject
{
    Q_OBJECT
public:
    explicit BondHandler(QObject *parent = 0);
    ~BondHandler();

    void Init();

protected:

    BaseMySql *bond_db;


signals:

public slots:
};

#endif // BONDHANDLER_H
