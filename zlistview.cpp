#include "zlistview.h"
#include "editor.h"
#include <DGuiApplicationHelper>
#include <DThemeManager>
#include <DPlatformTheme>
#include <DPushButton>
#include <QtWidgets>
#include <QDebug>

DGUI_USE_NAMESPACE
DTK_USE_NAMESPACE
ZListView::ZListView(QWidget *parent):QListView(parent)
{
//    setMouseTracking(true);
    setDragEnabled(false);
    setBatchSize(100);
    setLayoutMode(QListView::Batched);
    setFlow(QListView::TopToBottom);
    setWrapping(false);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformItemSizes(true);
    setItemDelegate(new ItemDelegate(this));
    setSpacing(2);
    viewport()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
}
QList<ZNote> ZListView::selection()const
{
    auto indexes=selectedIndexes();
    QList<ZNote> rt;
    for(auto i:indexes)
        rt.push_back(i.data(Qt::UserRole).value<ZNote>());
    return rt;
}
void ZListView::clearSelectionExt()
{
    if(!selectionModel()->hasSelection()) return ;
    int front=selectedIndexes().front().row();
    clearSelection();
    if(model()->rowCount()>front)
        setCurrentIndex(model()->index(front,0));
    else if(model()->rowCount()!=0)setCurrentIndex(model()->index(front-1,0));
    else emit listEmptied();
}
void ZListView::setCurrentIndex(const QModelIndex &cur)
{
//    clearSelection();
    selectionModel()->setCurrentIndex(cur,QItemSelectionModel::SelectCurrent);
    emit activeChange(cur);
//    QListView::setCurrentIndex(cur);
}
void ZListView::setNoBackground(bool b)
{
    if(!b)return ;
    viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);
}

int ZListModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(items.size());
}
QVariant ZListModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<"call data"<<index.row()<<"role"<<role;
    if(!index.isValid())return QVariant();
    auto rt=items.getKth(index.row()+1);
    if(role==Qt::UserRole)
    {
        return QVariant::fromValue(rt);
    }
    else if(role==UpdateTime)
    {
        return QVariant::fromValue(rt.lastModified());
    }
    else if(role==Html)
    {
        return QVariant::fromValue(rt.getHtml());
    }
    else if(role==Overview)
    {
        return QVariant::fromValue(rt.getOverview());
    }
    return QVariant();
}
bool ZListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count);

    endInsertRows();
}
bool ZListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())return false;
    if(role==Overview || role==Html)
    {
//        qDebug()<<"updating html/overview";
        auto str=value.value<QString>();
        items.modifyKth(index.row()+1,[str,role](ZNote *key){
            if(role==Overview) key->setOverview(str);
            else if(role==Html) key->setHtml(str);
            else qDebug()<<"ZListModel::setData error";
        });
    }
    else if(role==UpdateTime)
    {
        auto itemData=index.data(Qt::UserRole).value<ZNote>();
        items.erase(itemData);
        itemData.commitChange();
        items.insert(itemData);
    }
    emit dataChanged(index,index,QVector<int>({Qt::UserRole}));
    return true;
}
void ZListModel::appendRow(const ZNote &value)
{
    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
//    beginInsertRows(QModelIndex(),0,rowCount()+1);
    items.insert(value);
//   changePersistentIndexList();
//    endInsertRows();
    emit layoutChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
}
void ZListModel::removeRow(const ZNote &value)
{
    emit layoutAboutToBeChanged();
//    beginInsertRows(parent);
    items.erase(value);
//    changePersistentIndexList();
//    endInsertRows();
    emit layoutChanged();
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
    qDebug()<<"max depth"<<items.depth();
}

ItemDelegate::ItemDelegate(QWidget *parent):QStyledItemDelegate(parent){}
void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())return ;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect rect(option.rect);
    rect.adjust(5,5,-5,-5);
    QPainterPath path;
    path.addRoundRect(QRectF(rect),30);
    painter->setOpacity(1);
    if(option.state.testFlag(QStyle::State_Selected))
        painter->fillPath(path,option.palette.dark());
    else if(option.state.testFlag(QStyle::State_MouseOver))
        painter->fillPath(path,option.palette.midlight());
    else painter->fillPath(path,option.palette.base());

    if(option.state.testFlag(QStyle::State_Selected))
    {
        QRect borderRect(rect);
        borderRect.adjust(-3,-3,3,3);
        QPainterPath borderPath;
        borderPath.addRoundRect(QRectF(borderRect),33);
        painter->setPen(QPen(option.palette.highlight(),2));
        painter->drawPath(borderPath);
    }

    auto data=index.data(Qt::UserRole).value<ZNote>();
    rect.adjust(5,5,-5,-5);
    QRect mainRect(rect),infoRect(rect);
    mainRect.adjust(0,0,0,-20);
    infoRect.adjust(0,40,0,0);
    painter->setPen(QPen(option.palette.text(),1));
    painter->drawText(QRectF(mainRect),QFontMetrics(painter->font()).elidedText(data.getOverview(),Qt::ElideRight,500));
    auto font=painter->font();
    font.setItalic(true);
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(QPen(Qt::gray,1));
    painter->drawText(QRectF(infoRect),QString("%1 %2   %3 %4").arg(QObject::tr("上次修改"),data.getUpdateTime(),QObject::tr("创建于"),data.getCreateTime()));

    painter->restore();
}
QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(295,80);
}
