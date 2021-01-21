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
    void setModel(QAbstractItemModel* m);
    void setSearchResult(SearchResult* result);

signals:
    void selectItem(const QModelIndex& cur);

protected:
    void resizeEvent(QResizeEvent* e) override;
    bool eventFilter(QObject* o, QEvent* e) override;
public slots:

private:
    DSearchEdit* input;
    SearchResult* result;
    bool eatFocusOut;
private slots:
};

#endif // SEARCHWIDGET_H
