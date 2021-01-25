#ifndef ZLISTVIEW_H
#define ZLISTVIEW_H

#include "znote.h"
#include <QListView>
#include <QStyledItemDelegate>

class ZListView : public QListView {
    Q_OBJECT
public:
    ZListView(QWidget* parent = nullptr);
    QList<ZNote> selectionNotes() const;
    QList<QModelIndex> selection() const;
    void clearSelectionExt();
    void setCurrentIndex(const QModelIndex& cur);
    void setNoBackground(bool b);
signals:
    void currentIndexChanged(const QModelIndex& cur);
    void listEmptied();
};

class ItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ItemDelegate(QWidget* parent = nullptr);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // ZLISTVIEW_H
