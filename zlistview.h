#ifndef ZLISTVIEW_H
#define ZLISTVIEW_H

#include <DSimpleListView>
#include <QStackedLayout>
#include "znote.h"

DWIDGET_USE_NAMESPACE

class ZListView : public DSimpleListView
{
    Q_OBJECT
public:
    ZListView(DSimpleListView *parent=nullptr);
    QColor borderColor;
    QColor themeColor;
    void refresh();
    void addItems(QList<DSimpleListItem*> items);
    void clearItems();
public slots:
    void addButtonOnClick();
    void popupMenu(QPoint pos, QList<DSimpleListItem*> items);
signals:
    void addButtonClicked();
    void removeItemsTriggered(QList<DSimpleListItem*>);
private:
    QWidget* initAddButton();
    QLayout* initAddLayer();
    QList<DSimpleListItem*> itemsAll;
};

class ZListItem:public DSimpleListItem
{
    Q_OBJECT
public:
    ZListItem(ZNote *src,QColor c);
    bool sameAs(DSimpleListItem *item);
    void drawBackground(QRect rect,QPainter *painter,int index,bool isSelect,bool isHover);
    void drawForeground(QRect rect,QPainter *painter,int column,int index,bool isSelect,bool isHover);
    ZNote* data()const;
    void updateText();
    QDateTime lastModified()const;
    static bool sortByDateTime(const DSimpleListItem *a, const DSimpleListItem *b, bool descadingSort);
protected:
    QString text;
    ZNote *note;
    QColor color;
private:
    int index;
};

#endif // ZLISTVIEW_H
