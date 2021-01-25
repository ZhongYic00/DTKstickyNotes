#ifndef ZLISTMODEL_H
#define ZLISTMODEL_H

#include "fhqtreap.cpp"
#include "znote.h"
#include <QAbstractItemModel>
#include <QTextDocumentFragment>

typedef ZNote InnerIndex;
Q_DECLARE_METATYPE(QTextDocumentFragment)
class ZListModel : public QAbstractListModel {
public:
    ZListModel(QObject* parent = nullptr);
    enum roles {
        Abstract = Qt::UserRole + 1,
        Html = Qt::UserRole + (1 << 1),
        UpdateTime = Qt::UserRole + (1 << 2),
        Attachment = Qt::UserRole + (1 << 3),
        Content = Abstract | Html,
        IndexRole = UpdateTime
    };
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant data(const InnerIndex& idx, int role = Qt::DisplayRole) const;
    inline bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole); //pre-process
    inline InnerIndex setData(const InnerIndex& idx, const QVariant& value, int role = Qt::EditRole); //pre-process
    void appendRow(const ZNote& value);
    void removeRow(const ZNote& value);
    inline QModelIndex latestIndex() const;
    QList<ZNote> exportAll() const;
    inline QModelIndex indexOf(const InnerIndex& idx);
#ifndef RELEASE
    void dbg();
#endif

private:
    Treap<ZNote> items;
    QVariant dataRole(const ZNote& rt, int role) const;
    InnerIndex setData(const QModelIndex& index, const InnerIndex& idx, const QVariant& value, const int& role); //only about user-role
};
bool ZListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;
    if (role < Qt::UserRole)
        return QAbstractItemModel::setData(index, value, role);
    auto idx = data(index, Qt::UserRole).value<InnerIndex>();
    setData(index, idx, value, role ^ Qt::UserRole);
    return true;
}
InnerIndex ZListModel::setData(const InnerIndex& idx, const QVariant& value, int role)
{
    //    assert(role > Qt::UserRole);
    auto index = indexOf(idx);
    return setData(index, idx, value, role ^ Qt::UserRole);
}
QModelIndex ZListModel::indexOf(const InnerIndex& idx)
{
    return index(items.queryIndex(idx) - 1); //Kth in items refers to (K-1)th in model
}

QModelIndex ZListModel::latestIndex() const { return index(0); }
#endif // ZLISTMODEL_H
