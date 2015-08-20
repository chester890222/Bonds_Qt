#include "bondinfo.h"
#include "ui_bondinfo.h"

BondInfo::BondInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BondInfo)
{
    ui->setupUi(this);
}

BondInfo::~BondInfo()
{
    delete ui;
}
