#include "bondinfo.h"
#include "ui_bondinfo.h"

#include <QDebug>


BondInfo::BondInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BondInfo)
{
    qDebug() << Q_FUNC_INFO;
    ui->setupUi(this);
    bondhandler = BondHandler::getInstance();
    bondhandler->init();

    model_bonds_info_table = new Model_Bonds_Info_Table();
    model_bonds_info_table->setCurCol(QStringList()<<"WindCode"<<"Bond Name" << "Last");
    model_bonds_info_table->setCurData(bondhandler->getBondPoolInstance()->getBondMap());
    ui->tableView_Bonds_Info->setModel(model_bonds_info_table);
    connect(bondhandler->getBondPoolInstance(),SIGNAL(signal_RealtimeDataUpdate(QStringList)), model_bonds_info_table, SLOT(slot_refreshData()));

}

BondInfo::~BondInfo()
{
    qDebug() <<Q_FUNC_INFO;
    if (bondhandler != NULL) {
//        bondhandler->clear();
        delete bondhandler;
        bondhandler = NULL;
    }
    if (model_bonds_info_table != NULL) {
        delete model_bonds_info_table;
        model_bonds_info_table = NULL;
    }


    delete ui;
}
