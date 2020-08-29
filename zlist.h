#ifndef ZLIST_H
#define ZLIST_H
#include <QtWidgets>
#include "znote.h"
#include "zlistview.h"

class ZList:public QWidget
{
    Q_OBJECT
public:
    ZList(QWidget *parent=nullptr);
    void addItem(const ZNote &item);
    void addItems(const QList<ZNote> &items);
    void removeItem(const ZNote &item);
    void removeItems(const QList<ZNote> &items);
    QList<ZNote> getDataList() const;
    void setCurrentOverview(const QString &overview);
    void setCurrentHtml(const QString &html);
    void setCurrentIndex(const QModelIndex &cur);
    void commitChange(bool trace=true);
    void popupMenu(const QPoint &pos, const QList<ZNote> &selection);
    ZListModel* getModel();
signals:
    void addButtonClicked();
    void currentChanged(const QModelIndex &cur);
    void listEmptied();
/*public slots:
    void popupMenu();*/
private:
    ZListView *listview;
    ZListModel *model;
    QColor themeColor;
    int curRow;
    bool haveChange;

    QWidget* initAddButton();
    QLayout* initAddLayer();
    void setCur(const QModelIndex &idx);
};

#endif // ZLIST_H
