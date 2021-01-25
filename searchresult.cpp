#include "searchresult.h"
#include "daemon.h"

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
    view->setSelectionMode(QAbstractItemView::SingleSelection);
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

    setWindowFlags(Qt::Drawer);
    setSearchEdit(edit);

    model->setDynamicSortFilter(false);
    model->setFilterRole(ZListModel::Abstract);
    connect(view, &QListView::clicked, [=](const QModelIndex& index) {
        auto idx = index.data(ZListModel::IndexRole).value<InnerIndex>();
        if (index.data(ZListModel::Attachment).value<bool>()) {
            //attached
            emit Daemon::instance()->activateMainwindow(idx);
        } else {
            //detached
            emit Daemon::instance()->activateStickyNote(idx);
        }
        view->clearSelection();
    });
}
void SearchResult::filter(const QString& str)
{
    qDebug() << "filterString" << str;
    if (str != "") {
        if (!isVisible()) {
            qDebug() << "show";
            setVisible(true);
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
    //    qDebug() << searchEdit->focusPolicy() << searchEdit->focusProxy() << focusProxy();
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
