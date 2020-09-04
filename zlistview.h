#ifndef ZLISTVIEW_H
#define ZLISTVIEW_H

#include <QListView>
#include <QStyledItemDelegate>
#include "znote.h"

class ZListView: public QListView
{
    Q_OBJECT
public:
    ZListView(QWidget *parent=nullptr);
    QList<ZNote> selectionNotes() const;
    QList<QModelIndex> selection() const;
    void clearSelectionExt();
    void setCurrentIndex(const QModelIndex &cur);
    void setNoBackground(bool b);
signals:
    void curIndexChanged(const QModelIndex &cur);
    void listEmptied();
};

class ItemDelegate: public QStyledItemDelegate
{
public:
    ItemDelegate(QWidget *parent=nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ZLISTVIEW_H
