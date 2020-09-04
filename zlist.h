#ifndef ZLIST_H
#define ZLIST_H
#include <QtWidgets>
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <DPlatformTheme>
#include "znote.h"
#include "zlistview.h"
#include "zlistmodel.h"
#include "roundedwidgets.h"
#include "daemon.h"

class ZList:public QWidget
{
    Q_OBJECT
public:
    ZList(Daemon *d,QWidget *parent=nullptr);
    void addItem(const ZNote &item);
    void addItems(const QList<ZNote> &items);
    void removeItem(const ZNote &item);
    void removeItems(const QList<ZNote> &items);
    QList<ZNote> getDataList() const;
    void setCurrentOverview(const QString &overview);
    void setCurrentHtml(const QString &html);
    void setCurrentIndex(const QModelIndex &cur);
    void commitChange(bool trace=true);
    void popupMenu(const QPoint &pos);
    QAbstractItemModel* getModel();
signals:
    void addButtonClicked();
    void currentChanged(const QModelIndex &cur);
    void listEmptied();
/*public slots:
    void popupMenu();*/
private:
    Daemon *daemon;
    ZListView *listview;
    QSortFilterProxyModel *model;
    QColor themeColor;
    QDateTime curIdx;
    bool haveChange;

    QWidget* initAddButton();
    QLayout* initAddLayer();
};

#endif // ZLIST_H
