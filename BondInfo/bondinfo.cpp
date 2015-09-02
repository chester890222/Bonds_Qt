#include "bondinfo.h"
#include "ui_bondinfo.h"

BondInfo::BondInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BondInfo)
{
    qDebug() << Q_FUNC_INFO;
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
