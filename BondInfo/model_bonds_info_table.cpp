#include "model_bonds_info_table.h"

Model_Bonds_Info_Table::Model_Bonds_Info_Table(QObject *parent) :
    QAbstractTableModel(parent)
{

}

Model_Bonds_Info_Table::~Model_Bonds_Info_Table() {
    delete CurData;
    CurData = NULL;
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
        else if("Bond Name" == currCol)
            return CurData->value(tmp_code)->name;
//        else if("OrderNumber" == currCol)
//            return CurData->at(index.row()).orderNumber;
//        else if("OrderDate" == currCol)
//            temp = QString::number(CurData->at(index.row()).orderDate);
//        else if("OrderTime" == currCol)
//            temp = QString::number(CurData->at(index.row()).orderTime);
//        else if("OrderStatus" == currCol)
//            temp = QString::number(CurData->at(index.row()).orderStatus);
//        else if("TradeSide" == currCol)
//            temp = QString::number(CurData->at(index.row()).tradeSide);
//        else if("OrderVolume" == currCol)
//            temp = QString::number(CurData->at(index.row()).orderVolume);
//        else if("OrderPrice" == currCol)
//            temp = QString::number(CurData->at(index.row()).orderPrice);
//        else if("TradedVolume" == currCol)
//            temp = QString::number(CurData->at(index.row()).tradedVolume);
//        else if("TradedPrice" == currCol)
//            temp = QString::number(CurData->at(index.row()).tradedPrice);
//        else if("CancelVolume" == currCol)
//            temp = QString::number(CurData->at(index.row()).cancelVolume);
//        else if("OrderFrozenFund"==currCol)
//            temp = QString::number(CurData->at(index.row()).orderFrozenFund);
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
    beginResetModel();
    endResetModel();
}














