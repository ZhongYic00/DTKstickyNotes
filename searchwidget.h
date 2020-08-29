#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <DSearchEdit>
#include "searchresult.h"

DWIDGET_USE_NAMESPACE
class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);
    void setSourceModel(QAbstractListModel *m);

signals:
    void changeCurrent(const QModelIndex &cur);
public slots:
    void changeSearchResult(const QString &text);
    void reset(bool b);
protected:
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    bool active;
    DSearchEdit *input;
    SearchResult *result;
private slots:
    void keepPopupPosition();
};

#endif // SEARCHWIDGET_H
