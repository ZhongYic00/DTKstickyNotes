#include "zlistview.h"
#include "editor.h"
#include <DGuiApplicationHelper>
#include <DPlatformTheme>
#include <DPushButton>
#include <QtWidgets>
#include <QDebug>

DGUI_USE_NAMESPACE
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

//    connect(this,&ZListView::currentChanged,[this](const QModelIndex &cur){emit activeChange(cur);qDebug()<<"receive curchanged";});
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

int ZListModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(items.size());
}
QVariant ZListModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<"call data"<<index.row()<<"role"<<role;
    if(!index.isValid())return QVariant();
    if(role==Qt::UserRole)
    {
        auto rt=items.getKth(index.row()+1);
        return QVariant::fromValue(rt);
    }
    else if(role==UpdateTime)
    {
        auto rt=items.getKth(index.row()+1);
        return QVariant::fromValue(rt.lastModified());
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
//    qDebug()<<"appendRow()"<<value.getOverview();
    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
//    beginInsertRows(QModelIndex(),0,rowCount()+1);
//    qDebug()<<0<<rowCount()+1;
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
//    qDebug()<<"call latestIndex"<<rowCount()-1;
    return index(0);
}
 QList<ZNote> ZListModel::exportAll() const
{
     qDebug()<<"call ZListModel::exportAll()";
    return QList<ZNote>::fromStdList(items.getAll());
}

ItemDelegate::ItemDelegate(QWidget *parent):QStyledItemDelegate(parent){}
void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    qDebug()<<"draw item";
    if(!index.isValid())return ;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect rect(option.rect);
    rect.adjust(5,5,-5,-5);
    QPainterPath path;
    path.addRoundRect(QRectF(rect),30);
    painter->setOpacity(1);
    if(option.state.testFlag(QStyle::State_Selected))
        painter->fillPath(path,QColor(Qt::white).darker(110));
    else if(option.state.testFlag(QStyle::State_MouseOver))
        painter->fillPath(path,QColor(Qt::white).darker(101));
    else painter->fillPath(path,Qt::white);

    if(option.state.testFlag(QStyle::State_Selected))
    {
        QRect borderRect(rect);
        borderRect.adjust(-3,-3,3,3);
        QPainterPath borderPath;
        borderPath.addRoundRect(QRectF(borderRect),33);
        painter->setPen(QPen(DGuiApplicationHelper::instance()->systemTheme()->activeColor(),2));
        painter->drawPath(borderPath);
    }

    auto data=index.data(Qt::UserRole).value<ZNote>();
    rect.adjust(5,5,-5,-5);
    QRect mainRect(rect),infoRect(rect);
    mainRect.adjust(0,0,0,-20);
    infoRect.adjust(0,40,0,0);
    painter->setPen(QPen(Qt::black,1));
    painter->drawText(QRectF(mainRect),data.getOverview());
    auto font=painter->font();
    font.setItalic(true);
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(QPen(Qt::gray));
    painter->drawText(QRectF(infoRect),QString("%1 %2   %3 %4").arg(QObject::tr("上次修改"),data.getUpdateTime(),QObject::tr("创建于"),data.getCreateTime()));
//    qDebug()<<data.lastModified()<<rect<<mainRect<<infoRect;
//    qDebug()<<"drawText"<<index.data(Qt::UserRole).value<ZNote>().getOverview();

    painter->restore();
}
QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(290,80);
}
