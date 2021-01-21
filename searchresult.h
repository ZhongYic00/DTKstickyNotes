#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include "roundedwidgets.h"
#include "zlistmodel.h"
#include "zlistview.h"
#include <DBlurEffectWidget>
#include <DLabel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
DWIDGET_USE_NAMESPACE
class SearchResult : public DBlurEffectWidget {
    Q_OBJECT
public:
    explicit SearchResult(QWidget* parent, QWidget* searchEdit = nullptr);
    inline void setModel(QAbstractItemModel* m);
    void setSearchEdit(QWidget* edit);
signals:
public slots:
    void filter(const QString& str);

protected:
    //    void focusInEvent(QFocusEvent* e) override;

private:
    ZListView* view;
    QWidget* searchEdit;
    QSortFilterProxyModel* model;
};
void SearchResult::setModel(QAbstractItemModel* m) { model->setSourceModel(m); }
#endif // SEARCHRESULT_H
