#include "zlist.h"
#include "editor.h"
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <DPlatformTheme>

DGUI_USE_NAMESPACE
ZList::ZList(QWidget *parent):QWidget(parent),curRow(-1),haveChange(false)
{
    listview.setModel(&model);
    listview.setObjectName("listview");

    DGuiApplicationHelper *guiAppHelp = DGuiApplicationHelper::instance();
    themeColor=guiAppHelp->systemTheme()->activeColor();
    setLayout(initAddLayer());

    listview.setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(this,&ZList::rightClickItems,this,&ZList::popupMenu);
    connect(&listview,&QListView::customContextMenuRequested,[this](const QPoint &pos){
        popupMenu(mapToGlobal(pos),listview.selection());
    });
    connect(&listview,&QListView::clicked,[this](const QModelIndex &cur){emit currentChanged(cur);});
    connect(&listview,&ZListView::activeChange,this,&ZList::currentChanged);
}
QWidget* ZList::initAddButton()
{
    auto *layer=new TransparentWidget(this);
    layer->setObjectName("toolBarLayer");

    QVBoxLayout *layout=new QVBoxLayout(layer);
    layer->setLayout(layout);
    layout->addStretch();

    QHBoxLayout *bar=new QHBoxLayout(layer);
    layout->addLayout(bar);
    layout->addSpacing(10);

    QIcon ico(":/images/add-btn");
    auto btn=new DPushButton(ico,"",this);
    btn->setObjectName("listAddButton");
    btn->setIconSize(QSize(50,50));
    btn->resize(50,50);
    btn->setStyleSheet(
                QString(
                    "#listAddButton{"
                    "border-radius:25;"
                    "background-color:%1;"
                    "margin:10;"
                    "}"
                    "#listAddButton:hover{"
                    "border-radius:25;"
                    "background-color:%2;"
                    "margin:10;"
                    "}"
                    "#listAddButton:pressed{"
                    "border-radius:25;"
                    "background-color:%3;"
                    "margin:10;"
                    "}").arg(themeColor.name(QColor::HexRgb),themeColor.darker(110).name(QColor::HexRgb),themeColor.darker(150).name(QColor::HexRgb)));

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(btn);
    effect->setOffset(0,0);
    effect->setColor(QColor(0,0,0,150));
    effect->setBlurRadius(10);
    btn->setGraphicsEffect(effect);

    connect(btn,&DPushButton::clicked,[this](){addItem(ZNote());});
    connect(btn,&QPushButton::clicked,[this](){
        auto i=model.latestIndex();
        listview.setCurrentIndex(i);
        emit currentChanged(i);
    });
    connect(this,&ZList::currentChanged,this,&ZList::setCur);
    connect(&listview,&ZListView::listEmptied,[this](){emit listEmptied();});

    bar->addStretch();
    bar->addWidget(btn);
    return layer;
}
QLayout* ZList::initAddLayer()
{
    QStackedLayout *layout=new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->addWidget(initAddButton());
    layout->addWidget(&listview);
    return layout;
}
void ZList::addItem(const ZNote &item)
{
    model.appendRow(item);
}
void ZList::addItems(const QList<ZNote> &items)
{
    listview.setUpdatesEnabled(false);
    for(auto i:items)
        addItem(i);
    listview.setUpdatesEnabled(true);
}
void ZList::removeItem(const ZNote &item)
{
    model.removeRow(item);
}
void ZList::removeItems(const QList<ZNote> &items)
{
    for(auto i:items)
        removeItem(i);
}
QList<ZNote> ZList::getDataList() const
{
    return model.exportAll();
}
void ZList::setCurrentOverview(const QString &overview)
{
    haveChange=true;
    auto curIndex=listview.currentIndex();
    model.setData(curIndex,QVariant::fromValue(overview),ZListModel::Overview);
}
void ZList::setCurrentHtml(const QString &html)
{
    haveChange=true;
    auto curIndex=listview.currentIndex();
    model.setData(curIndex,QVariant::fromValue(html),ZListModel::Html);
}
void ZList::commitChange(bool trace)
{
    auto curIndex=model.index(curRow);
    model.setData(curIndex,QVariant(),ZListModel::UpdateTime);
    if(trace)listview.setCurrentIndex(model.index(0));
    haveChange=false;
}
void ZList::setCur(const QModelIndex &idx)
{
    if(curRow!=-1 && haveChange)commitChange(false);
    curRow=idx.row();
    haveChange=false;
}
void ZList::popupMenu(const QPoint &pos, const QList<ZNote> &selection)
{
    QMenu *menu=new QMenu();
    QAction *removeAction=new QAction(QIcon(":/images/trash-empty"),tr("删除"),menu);
    connect(removeAction,&QAction::triggered,[&selection,this](){removeItems(selection);listview.clearSelectionExt();});
    menu->addAction(removeAction);
    menu->exec(pos);
}
