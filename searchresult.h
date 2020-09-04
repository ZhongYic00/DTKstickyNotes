#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QLabel>
#include "zlistview.h"
#include "zlistmodel.h"
#include "roundedwidgets.h"

class SearchResult : public QWidget
{
    Q_OBJECT
public:
    explicit SearchResult(QWidget *parent);
    void reset();
    void filter(const QString &str);
    void setSrcModel(QAbstractItemModel *m);
signals:
    void widgetMoving();
    void changeCurrent(const QModelIndex &cur);
public slots:
protected:
    void moveEvent(QMoveEvent *event);
private:
    QSortFilterProxyModel *model;
    ZListView *view;
};

#endif // SEARCHRESULT_H
