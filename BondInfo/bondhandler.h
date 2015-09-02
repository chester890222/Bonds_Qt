#ifndef BONDHANDLER_H
#define BONDHANDLER_H

#include <QObject>
#include "basemysql.h"
#include "BaseWindQuant.h"
#include "bondpool.h"

class BondHandler : public QObject
{
    Q_OBJECT
public:
    explicit BondHandler(QObject *parent = 0);
    ~BondHandler();

    void init();
    int clear();
    void selectBondFromDb();




//protected:

    BaseMySql *bond_db;
    BondPool *bondPool;

public:
    static BondHandler *getInstance();
    BondPool *getBondPoolInstance();

private:
    static BondHandler g_instance;

signals:

public slots:
};

#endif // BONDHANDLER_H
