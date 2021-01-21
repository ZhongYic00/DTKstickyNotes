#include "searchresult.h"

SearchResult::SearchResult(QWidget* parent, QWidget* edit)
    : DBlurEffectWidget(parent)
    , view(nullptr)
    , searchEdit(nullptr)
    , model(new QSortFilterProxyModel(this))
{
    setObjectName("searchResult");
    view = new ZListView(this);
    view->setNoBackground(true);
    view->setModel(model);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(new DLabel("Search Results", this));
    layout->addWidget(view);
    setLayout(layout);

    setRadius(15);
    setBlurRectXRadius(18);
    setBlurRectYRadius(18);
    setBlurEnabled(true);
    setMode(DBlurEffectWidget::GaussianBlur);

    //setParent(nullptr);
    setWindowFlags(Qt::Drawer);
    //setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowModality(Qt::NonModal);
    setSearchEdit(edit);

    model->setDynamicSortFilter(false);
    model->setFilterRole(ZListModel::Overview);
    //connect(view, &QListView::clicked, [=](const QModelIndex& cur) { emit changeCurrent(model->mapToSource(cur)); });
}
void SearchResult::filter(const QString& str)
{
    qDebug() << "filterString" << str;
    if (str != "") {
        if (!isVisible()) {
            qDebug() << "show";
            setVisible(true);
            //searchEdit->setFocus();
        }
        model->setFilterFixedString(str);
    } else {
        if (!model->filterRegExp().isEmpty()) {
            qDebug() << "hide";
            setVisible(false);
        }
    }
}
void SearchResult::setSearchEdit(QWidget* edit)
{
    if (!edit)
        return;
    searchEdit = edit;
    searchEdit->setParent(this);
    auto originPolicy = Qt::NoFocus;
    originPolicy = searchEdit->focusPolicy();
    setFocusPolicy(Qt::NoFocus);
    searchEdit->setFocusPolicy(originPolicy);
    setFocusProxy(searchEdit);
    qDebug() << searchEdit->focusPolicy() << searchEdit->focusProxy() << focusProxy();
}
//SearchResult::SearchResult(QWidget* parent, QWidget* edit)
//    : ZListView(parent)
//{
//    setSearchEdit(edit);
//    hide();
//    setParent(nullptr);
//    setWindowFlag(Qt::Popup);
//    setWindowModality(Qt::NonModal);
//}
//void SearchResult::setSearchEdit(QWidget* edit)
//{
//    if (!edit)
//        return;
//    searchEdit = edit;
//    auto originPolicy = Qt::NoFocus;
//    originPolicy = searchEdit->focusPolicy();
//    setFocusPolicy(Qt::NoFocus);
//    searchEdit->setFocusPolicy(originPolicy);
//    setFocusProxy(searchEdit);
//    qDebug() << searchEdit->focusPolicy() << searchEdit->focusProxy() << focusProxy();
//}
//void SearchResult::filter(const QString& str)
//{
//    qDebug() << size();
//    if (!isVisible()) {
//        qDebug() << "show";
//        show();
//        searchEdit->setFocus();
//    }
//    //    model->setFilterFixedString(str);
//}
