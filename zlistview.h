#ifndef ZLISTVIEW_H
#define ZLISTVIEW_H

#include <QListView>
#include <QStyledItemDelegate>
#include "znote.h"
#include "fhqtreap.cpp"

class ZListModel:public QAbstractListModel
{
public:
    enum roles{Overview=Qt::UserRole+1,Html=Qt::UserRole+2,UpdateTime=Qt::UserRole+3};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void appendRow(const ZNote &value);
    void removeRow(const ZNote &value);
    QModelIndex latestIndex() const;
    QList<ZNote> exportAll() const;
private:
    Treap<ZNote> items;
};

class ZListView: public QListView
{
    Q_OBJECT
public:
    ZListView(QWidget *parent=nullptr);
    QList<ZNote> selection() const;
    void clearSelectionExt();
    void setCurrentIndex(const QModelIndex &cur);
signals:
    void activeChange(const QModelIndex &cur);
    void listEmptied();
};

#endif // ZLISTVIEW_H

class ItemDelegate: public QStyledItemDelegate
{
public:
    ItemDelegate(QWidget *parent=nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
