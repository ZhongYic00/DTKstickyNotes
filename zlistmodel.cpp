#include "zlistmodel.h"

ZListModel::ZListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}
int ZListModel::rowCount(const QModelIndex& parent) const
{
    //    qDebug() << __FUNCTION__ << parent << parent.isValid() << items.size();
    if (parent.isValid())
        return 0;
    return items.size();
}
QVariant ZListModel::dataRole(const ZNote& rt, int role) const
{
    switch (role) {
    case Qt::UserRole:
        return QVariant::fromValue(rt);
    case UpdateTime:
        return QVariant::fromValue(InnerIndex(rt.lastModified()));
    case Html:
        return rt.getHtml();
    case Abstract:
        return rt.getAbstract();
    case Attachment:
        return rt.isAttached();
    default:
        return QVariant();
    }
}
QVariant ZListModel::data(const QModelIndex& index, int role) const
{
    //    qDebug() << "call" << __func__ << index << role;
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
    //    qDebug() << __FUNCTION__ << idx << role;
    if (role == Qt::UserRole) {
        items.erase(items.getKth(idx));
        items.insert(value.value<ZNote>());
        emit dataChanged(indexOf(value.value<ZNote>().lastModified()), index, QVector<int>({ Qt::UserRole }));
    }
    if (role & Content || role & Attachment) { //bugs may occur
        items.modifyKth(idx, [&](ZNote* key) {
            if (role & Content)
                key->setContent(value.value<QTextDocumentFragment>());
            if (role & Attachment)
                key->toggleAttach();
        });
        emit dataChanged(index, index, QVector<int>({ Qt::UserRole }));
    }
    if (role & UpdateTime) {
        auto itemData = items.getKth(idx);
        items.erase(itemData);
        itemData.commitChange();
        items.insert(itemData);
        emit dataChanged(indexOf(itemData.getUpdateTimeRaw()), index, QVector<int>({ Qt::UserRole }));
        return InnerIndex(itemData.lastModified());
    }
    return InnerIndex();
}
void ZListModel::appendRow(const ZNote& value)
{
    int pos = items.queryIndex(value);
    //    qDebug() << __FUNCTION__ << value.lastModified() << "at" << pos;
    beginInsertRows(QModelIndex(), pos, pos);
    //emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
    items.insert(value);
    //emit layoutChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
    endInsertRows();
}
void ZListModel::removeRow(const ZNote& value)
{
    int index = items.queryIndex(value) - 1;
    beginRemoveRows(QModelIndex(), index, index);
    //emit layoutAboutToBeChanged();
    //    qDebug() << items;
    items.erase(value);
    //emit layoutChanged();
    endRemoveRows();
}
QList<ZNote> ZListModel::exportAll() const
{
    //     qDebug()<<"call ZListModel::exportAll()";
    return QList<ZNote>::fromStdList(items.getAll());
}
void ZListModel::dbg()
{
    qDebug() << rowCount() << "items in model:";
    for (auto i = 0; i < rowCount(); i++) {
        auto idx = index(i);
        qDebug() << idx.data(Qt::UserRole).value<ZNote>() << "attachment:" << idx.data(Attachment);
    }
}
