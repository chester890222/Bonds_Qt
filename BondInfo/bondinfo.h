#ifndef BONDINFO_H
#define BONDINFO_H

#include <QWidget>
#include "basemysql.h"

#include "bondhandler.h"
#include "model_bonds_info_table.h"


namespace Ui {
class BondInfo;
}

class BondInfo : public QWidget
{
    Q_OBJECT

public:
    explicit BondInfo(QWidget *parent = 0);
    ~BondInfo();

protected:
    BondHandler *bondhandler;

    Model_Bonds_Info_Table *model_bonds_info_table;


private:
    Ui::BondInfo *ui;
};

#endif // BONDINFO_H
