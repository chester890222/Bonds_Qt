#ifndef BONDINFO_H
#define BONDINFO_H

#include <QWidget>
#include "basemysql.h"

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


private:
    Ui::BondInfo *ui;
};

#endif // BONDINFO_H
