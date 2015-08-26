#ifndef BONDINFO_H
#define BONDINFO_H

#include <QWidget>
#include "basemysql.h"

#include "bondhandler.h"

#include "bondhandler.h"

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

private:
    Ui::BondInfo *ui;
};

#endif // BONDINFO_H
