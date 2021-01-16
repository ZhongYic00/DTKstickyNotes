#include "zlistmodel.h"

ZListModel::ZListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}
int ZListModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(items.size());
}
QVariant ZListModel::dataRole(const ZNote& rt, int role) const
{
    if (role == Qt::UserRole) {
        return QVariant::fromValue(rt);
    } else if (role == UpdateTime) {
        return QVariant::fromValue(rt.lastModified());
    } else if (role == Html) {
        return QVariant::fromValue(rt.getHtml());
    } else if (role == Overview) {
        return QVariant::fromValue(rt.getOverview());
    } else if (role == DisplayType) {
        return QVariant::fromValue(rt.getAttach());
    }
    return QVariant();
}
QVariant ZListModel::data(const QModelIndex& index, int role) const
{
    //    qDebug()<<"call data"<<index.row()<<"role"<<role;
    if (!index.isValid())
        return QVariant();
    auto rt = items.getKth(index.row() + 1);
    return dataRole(rt, role);
}
QVariant ZListModel::data(const InnerIndex& idx, int role) const
{
    auto rt = items.getKth(idx);
    return dataRole(rt, role);
}
InnerIndex ZListModel::setData(const QModelIndex& index, const InnerIndex& idx, const QVariant& value, const int& role)
{
    if (role == Overview || role == Html || role == DisplayType) {
        auto str = value.value<QString>();
        items.modifyKth(idx, [str, role](ZNote* key) {
            if (role == Overview)
                key->setOverview(str);
            else if (role == Html)
                key->setHtml(str);
            else if (role == DisplayType)
                key->toggleAttach();
            else
                qDebug() << "ZListModel::setData error";
        });
        emit dataChanged(index, index, QVector<int>({ Qt::UserRole }));
    } else if (role == UpdateTime) {
        auto itemData = items.getKth(idx);
        items.erase(itemData);
        itemData.commitChange();
        items.insert(itemData);
        emit dataChanged(indexOf(itemData.getUpdateTimeRaw()), index, QVector<int>({ Qt::UserRole }));
        return itemData.getUpdateTimeRaw();
    }
    return InnerIndex();
}
void ZListModel::appendRow(const ZNote& value)
{
    beginInsertRows(QModelIndex(), 0, 0);
    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
    items.insert(value);
    //   changePersistentIndexList();
    emit layoutChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
    endInsertRows();
}
void ZListModel::removeRow(const ZNote& value)
{
    int index = items.queryIndex(value) - 1;
    beginRemoveRows(QModelIndex(), index, index);
    emit layoutAboutToBeChanged();
    items.erase(value);
    //    changePersistentIndexList();
    emit layoutChanged();
    endRemoveRows();
}
QModelIndex ZListModel::latestIndex() const
{
    return index(0);
}
QList<ZNote> ZListModel::exportAll() const
{
    //     qDebug()<<"call ZListModel::exportAll()";
    return QList<ZNote>::fromStdList(items.getAll());
}
void ZListModel::dbg()
{
    qDebug() << "max depth" << items.depth();
}
