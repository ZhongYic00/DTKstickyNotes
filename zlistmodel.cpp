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
    switch (role) {
    case Qt::UserRole:
        return QVariant::fromValue(rt);
    case UpdateTime:
        return rt.lastModified();
    case Html:
        return rt.getHtml();
    case Overview:
        return rt.getOverview();
    case Attachment:
        return rt.isAttached();
    default:
        return QVariant();
    }
}
QVariant ZListModel::data(const QModelIndex& index, int role) const
{
    //    qDebug() << "call" << __func__ << index << role;
    //    qDebug()<<"call data"<<index.row()<<"role"<<role;
    if (!index.isValid())
        return QVariant();
    auto rt = items.getKth(index.row() + 1);
    return dataRole(rt, role);
}
QVariant ZListModel::data(const InnerIndex& idx, int role) const
{
    //    qDebug() << "call " << __func__ << endl;
    auto rt = items.getKth(idx);
    return dataRole(rt, role);
}
InnerIndex ZListModel::setData(const QModelIndex& index, const InnerIndex& idx, const QVariant& value, const int& role)
{
    qDebug(__FUNCTION__);
    if (role == Qt::UserRole) {
        items.erase(items.getKth(idx));
        items.insert(value.value<ZNote>());
        emit dataChanged(indexOf(value.value<ZNote>().lastModified()), index, QVector<int>({ Qt::UserRole }));
    }
    if (role == Overview || role == Html || role == Attachment) {
        auto str = value.value<QString>();
        items.modifyKth(idx, [str, role](ZNote* key) {
            if (role == Overview)
                key->setOverview(str);
            else if (role == Html)
                key->setHtml(str);
            else if (role == Attachment)
                key->toggleAttach();
            else
                qDebug() << "ZListModel::setData error";
        });
        emit dataChanged(index, index, QVector<int>({ role }));
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
    int pos = items.queryIndex(value);
    qDebug() << __FUNCTION__ << value.lastModified() << "at" << pos;
    beginInsertRows(QModelIndex(), pos, pos);
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
QList<ZNote> ZListModel::exportAll() const
{
    //     qDebug()<<"call ZListModel::exportAll()";
    return QList<ZNote>::fromStdList(items.getAll());
}
void ZListModel::dbg()
{
    qDebug() << "max depth" << items.depth();
}
