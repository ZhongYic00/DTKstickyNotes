#include "zlistview.h"
#include <QtWidgets>
#include <DGuiApplicationHelper>
#include <DPlatformTheme>

DGUI_USE_NAMESPACE
ZListView::ZListView(QWidget *parent):QListView(parent)
{
	model=new ZModel;
	setDragEnabled(false);
	setItemDelegate(new ZDelegate);
	setModel(model);
	setBatchSize(100);
	setLayoutMode(QListView::Batched);
	setFlow(QListView::TopToBottom);
	setWrapping(true);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//	ItemDelegate itemDelegate(this);
	setItemDelegate(ItemDelegate(this));
	setSpacing(10);
}
QList<ZNote> ZListView::selection()const
{
	auto indexes=selectedIndexes();
	QList<ZNote> rt;
	for(auto i:indexes)
		rt.push_back(data(i,Qt::UserRole).value<ZNote>());
	return rt;
}
void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(!index.isValid())return ;
	painter->save();
	
	QRect rect(option.rect);
	QPainterPath path;
	path.addRoundRect(QRectF(rect),30);
	painter->setOpacity(1);

	painter->restore();
}
ZListModel::ZListModel(QObject *parent):QAbstractListModel(parent){}
int rowCount(const QModelIndex &parent) const
{
	return items.count();
}
QVariant ZListModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())return QVariant();
	if(role==Qt::UserRole)
	{
		auto rt=items.getKth(index.row());
		return rt;
	}
	return QVariant();
}
bool ZListModel::insertRows(int row, int count, const QModelIndex &parent = QModelIndex())
{
	beginInsertRows();
	
	endInsertRows();
}
bool ZListModel::setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)
{
	if(!index.isValid())return false;
	auto itemData=index.data(Qt::UserRole).value<ZNote>();
	items.erase(itemData);
	items.insert(value.value<ZNote>());
	emit dataChanged(index,index,QVector<int>({Qt::UserRole}));
	return true;
}
void ZListModel::appendRow(const QVariant &value)
{
	emit layoutAboutToBeChanged();
	beginInsertRows();
	items.insert(value.value<ZNote>());
	changePersistentIndexList();
	endInsertRows();
	emit layoutChanged();
}
