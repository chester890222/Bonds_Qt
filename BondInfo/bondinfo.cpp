#include "bondinfo.h"
#include "ui_bondinfo.h"

#include <QDebug>


BondInfo::BondInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BondInfo)
{
    qDebug() << Q_FUNC_INFO;
    ui->setupUi(this);
    bondhandler = new BondHandler();
    bondhandler->init();

    model_bonds_info_table = new Model_Bonds_Info_Table();
    model_bonds_info_table->setCurCol(QStringList()<<"WindCode"<<"Bond Name" << "Last" << "Bid 1 Size" << "Bid 1" << "Ask 1" << "Ask 1 Size");
    model_bonds_info_table->setCurData(bondhandler->getBondPoolInstance()->getBondMap());
    ui->tableView_Bonds_Info->setModel(model_bonds_info_table);
    ui->tableView_Bonds_Info->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_Bonds_Info->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_Bonds_Info->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(bondhandler->getBondPoolInstance(),SIGNAL(signal_RealtimeDataUpdate(QStringList)), model_bonds_info_table, SLOT(slot_refreshData()));

    widget_single_info = new Widget_Single_Info(this);
    ui->dockWidget_single_info->setWidget(widget_single_info);

    connect(this,SIGNAL(signal_table_selected(BaseBond)),widget_single_info,SLOT(slot_refresh_data(BaseBond)));
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

void BondInfo::on_tableView_Bonds_Info_doubleClicked(const QModelIndex &index)
{
    QModelIndex ind = model_bonds_info_table->index(index.row(),0, QModelIndex());
    QString code =  model_bonds_info_table->data(ind, Qt::DisplayRole).toString();
    emit signal_table_selected(*bondhandler->getBondPoolInstance()->getBondMap()->value(code));

}
