#ifndef BONDHANDLER_H
#define BONDHANDLER_H

#include <QObject>
#include "basemysql.h"
#include "BaseWindQuant.h"
#include "bondpool.h"

//单例模式，只存在一个BondHandler对象
//线程安全
//控制程序流程、与数据库、服务器进行数据交互
class BondHandler : public QObject
{
    Q_OBJECT

public:
    //////////////////////////////////////////
    static BondHandler *getInstance();
    BondPool *getBondPoolInstance();
    //////////////////////////////////////////

    explicit BondHandler(QObject *parent = 0);
    ~BondHandler();

    void init();
    int clear();
    void selectBondFromDb();
    void selectBondFromDb(QStringList codesList);

    QStringList getBondCodes();


protected:
    bool isInit;

    BaseMySql *bond_db;
    BondPool *bondPool;

private:
    static BondHandler g_instance;


signals:

public slots:
};

#endif // BONDHANDLER_H
