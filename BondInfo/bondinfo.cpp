#include "bondinfo.h"
#include "ui_bondinfo.h"

BondInfo::BondInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BondInfo)
{
    ui->setupUi(this);
    bondhandler = new BondHandler();
    bondhandler->init();
}

BondInfo::~BondInfo()
{
    if (bondhandler != NULL) {
        delete bondhandler;
        bondhandler = NULL;
    }
    delete ui;
}
