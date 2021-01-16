#ifndef ZLISTMODEL_H
#define ZLISTMODEL_H

#include "fhqtreap.cpp"
#include "znote.h"
#include <QAbstractItemModel>

typedef ZNote InnerIndex;
class ZListModel : public QAbstractListModel {
public:
    ZListModel(QObject* parent = nullptr);
    enum roles { Overview = Qt::UserRole + 1,
        Html = Qt::UserRole + 2,
        UpdateTime = Qt::UserRole + 3,
        DisplayType = Qt::UserRole + 4 };
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant data(const InnerIndex& idx, int role = Qt::DisplayRole) const;
    //    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    inline bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    inline InnerIndex setData(const InnerIndex& idx, const QVariant& value, int role = Qt::EditRole);
    void appendRow(const ZNote& value);
    void removeRow(const ZNote& value);
    QModelIndex latestIndex() const;
    QList<ZNote> exportAll() const;
    void dbg();
    inline QModelIndex indexOf(const InnerIndex& idx);

private:
    Treap<ZNote> items;
    QVariant dataRole(const ZNote& rt, int role) const;
    InnerIndex setData(const QModelIndex& index, const InnerIndex& idx, const QVariant& value, const int& role);
};
bool ZListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;
    auto idx = data(index, Qt::UserRole).value<InnerIndex>();
    setData(index, idx, value, role);
    return true;
}
InnerIndex ZListModel::setData(const InnerIndex& idx, const QVariant& value, int role)
{
    auto index = indexOf(idx);
    return setData(index, idx, value, role);
}
QModelIndex ZListModel::indexOf(const InnerIndex& idx)
{
    return index(items.queryIndex(idx) - 1); //Kth in items refers to (K-1)th in model
}

#endif // ZLISTMODEL_H
