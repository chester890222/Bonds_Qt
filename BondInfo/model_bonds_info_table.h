#ifndef MODEL_BONDS_INFO_TABLE_H
#define MODEL_BONDS_INFO_TABLE_H

#include <QAbstractTableModel>
#include "bondhandler.h"

class Model_Bonds_Info_Table : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Model_Bonds_Info_Table(QObject *parent = 0);
    ~Model_Bonds_Info_Table();

    void setCurData(const QMap<QString,BaseBond *> *data);
    void setCurCol(QStringList col);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const QMap<QString,BaseBond *> *CurData;
    QStringList CurCol;


signals:

public slots:
    void slot_refreshData();
};

#endif // MODEL_BONDS_INFO_TABLE_H
