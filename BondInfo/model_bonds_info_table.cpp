#include "model_bonds_info_table.h"
#include <QDebug>

Model_Bonds_Info_Table::Model_Bonds_Info_Table(QObject *parent) :
    QAbstractTableModel(parent)
{

}

Model_Bonds_Info_Table::~Model_Bonds_Info_Table() {
    qDebug() << Q_FUNC_INFO;
//    delete CurData;
//    CurData = NULL;
}


int Model_Bonds_Info_Table::rowCount(const QModelIndex &parent) const {
    if (CurData != NULL) {
        return CurData->size();
    } else {
        return 0;
    }

}

int Model_Bonds_Info_Table::columnCount(const QModelIndex &parent) const {
    return CurCol.size();
}

QVariant Model_Bonds_Info_Table::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    QString currCol = CurCol[index.column()];
    QString temp;

    if(role==Qt::DisplayRole) {
        QString tmp_code = CurData->keys().at(index.row());
        if("WindCode"==currCol)
            return tmp_code;
        else if ("Bond Name" == currCol)
            return CurData->value(tmp_code)->name;
        else if ("Last" == currCol)
            return CurData->value(tmp_code)->realtimedata.rt_last;
        else if ("Ask 1" == currCol)
            return CurData->value(tmp_code)->realtimedata.rt_ask[0];
        else if ("Bid 1" == currCol)
            return CurData->value(tmp_code)->realtimedata.rt_bid[0];
        else if ("Ask 1 Size" == currCol)
            return CurData->value(tmp_code)->realtimedata.rt_asize[0];
        else if ("Bid 1 Size" == currCol)
            return CurData->value(tmp_code)->realtimedata.rt_bsize[0];
    }

    return QVariant(temp);
}



QVariant Model_Bonds_Info_Table::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role!=Qt::DisplayRole)
        return QVariant();

    if(orientation==Qt::Horizontal) {
        if(CurCol.size()>section)
            return CurCol[section];
        else
            return QString("Column %1").arg(section);
    }
    else
        return QVariant();

}



void Model_Bonds_Info_Table::setCurData(const QMap<QString,BaseBond *> *data) {
    CurData = data;
}

void Model_Bonds_Info_Table::setCurCol(QStringList col) {
    CurCol = col;
}

void Model_Bonds_Info_Table::slot_refreshData() {
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    endResetModel();
}














