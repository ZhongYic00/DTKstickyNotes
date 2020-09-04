#ifndef ZLISTMODEL_H
#define ZLISTMODEL_H

#include<QAbstractItemModel>
#include "znote.h"
#include "fhqtreap.cpp"

typedef QDateTime InnerIndex;
class ZListModel:public QAbstractListModel
{
public:
    ZListModel(QObject *parent=nullptr);
    enum roles{Overview=Qt::UserRole+1,Html=Qt::UserRole+2,UpdateTime=Qt::UserRole+3,DisplayType=Qt::UserRole+4};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant data(const InnerIndex &idx, int role = Qt::DisplayRole) const;
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    InnerIndex setData(const InnerIndex &idx, const QVariant &value, int role = Qt::EditRole);
    void appendRow(const ZNote &value);
    void removeRow(const ZNote &value);
    QModelIndex latestIndex() const;
    QList<ZNote> exportAll() const;
    void dbg();
    QModelIndex indexOf(const QDateTime &idx);
private:
    Treap<ZNote> items;
    QVariant dataRole(ZNote &rt, int role) const;
};

#endif // ZLISTMODEL_H
