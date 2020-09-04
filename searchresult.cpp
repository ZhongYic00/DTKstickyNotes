#include "searchresult.h"

SearchResult::SearchResult(QWidget *parent) : QWidget (parent), model(nullptr)
{
    setObjectName("searchResult");
    setWindowFlags(Qt::Drawer | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    auto visibleLayer=new RoundedWidget(this);
    visibleLayer->setRadius(10);
    visibleLayer->setShadow(5);
    visibleLayer->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    auto hiddenLayer=new QHBoxLayout(this);
    hiddenLayer->addWidget(visibleLayer);
    setLayout(hiddenLayer);

    view=new ZListView(this);
    view->setNoBackground(true);
    auto layout=new QVBoxLayout(this);
    auto subLayout=new QHBoxLayout(this);
    subLayout->addSpacing(20);
    subLayout->addWidget(new QLabel(tr("Search Results:")));
    layout->addLayout(subLayout);
    layout->addWidget(view);
    layout->setMargin(0);
    visibleLayer->setLayout(layout);

    model=new QSortFilterProxyModel(this);
    view->setModel(model);
    filter(""); //keep search result empty when focused first time

    connect(view,&QListView::clicked,[=](const QModelIndex &cur){
        emit changeCurrent(model->mapToSource(cur));
    });
}
void SearchResult::moveEvent(QMoveEvent *event)
{
    emit widgetMoving();
//    return QWidget::moveEvent(event);
}
void SearchResult::filter(const QString &str)
{
    if(!model) return ;
    model->setFilterRole(ZListModel::Overview);
    if(str.length())
        model->setFilterFixedString(str);
    else
        model->setFilterFixedString("\127");
}
void SearchResult::setSrcModel(QAbstractItemModel *m)
{
    model->setSourceModel(m);
}
