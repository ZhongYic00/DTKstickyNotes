#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "searchresult.h"
#include <DSearchComboBox>
#include <DSearchEdit>
#include <QWidget>

DWIDGET_USE_NAMESPACE
class SearchWidget : public QWidget {
    Q_OBJECT
public:
    explicit SearchWidget(QWidget* parent = nullptr);
    void setSourceModel(QAbstractItemModel* m);

signals:
    void changeCurrent(const QModelIndex& cur);
public slots:
    void changeSearchResult(const QString& text);
    void reset(bool b);

protected:
    void moveEvent(QMoveEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    bool active;
    bool freeze;
    DSearchComboBox* input;
    SearchResult* result;
private slots:
    void keepPopupPosition();
    void melt();
};

#endif // SEARCHWIDGET_H
