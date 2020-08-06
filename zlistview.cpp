#include "zlistview.h"
#include "editor.h"
#include <DGuiApplicationHelper>
#include <DPlatformTheme>
#include <DPushButton>
#include <QtWidgets>
#include <QDebug>

DGUI_USE_NAMESPACE
ZListView::ZListView(DSimpleListView *parent):DSimpleListView(parent)
{
    setFrame(true);
    setClipRadius(8);
    setFrame(true, QColor("#FF0000"), 0.5);
    setRowHeight(80);
    DGuiApplicationHelper *guiAppHelp = DGuiApplicationHelper::instance();
    borderColor=guiAppHelp->systemTheme()->activeColor();
    scrollbarColor="#"+QString::number(guiAppHelp->systemTheme()->shadow().rgb(),16);
    setLayout(initAddLayer());

    connect(this,&ZListView::rightClickItems,this,&ZListView::popupMenu);
    auto lst=new QList<SortAlgorithm>;
    lst->append(&ZListItem::sortByDateTime);
    setColumnSortingAlgorithms(lst,0,false);
    changeSortingStatus(0,false);
}
QWidget* ZListView::initAddButton()
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
                    "}").arg(borderColor.name(QColor::HexRgb),borderColor.darker(110).name(QColor::HexRgb),borderColor.darker(150).name(QColor::HexRgb)));

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(btn);
    effect->setOffset(0,0);
    effect->setColor(QColor(0,0,0,150));
    effect->setBlurRadius(10);
    btn->setGraphicsEffect(effect);

    connect(btn,&DPushButton::clicked,this,&ZListView::addButtonClicked);

    bar->addStretch();
    bar->addWidget(btn);
    return layer;
}
QLayout* ZListView::initAddLayer()
{
    QStackedLayout *layout=new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->addWidget(initAddButton());
    return layout;
}
void ZListView::addButtonOnClick()
{
    emit addButtonClicked();
}
void ZListView::popupMenu(QPoint pos, QList<DSimpleListItem*> items)
{
    QMenu *menu=new QMenu();
    QAction *removeAction=new QAction(QIcon(":/images/trash-empty"),tr("删除"),menu);
    connect(removeAction,&QAction::triggered,[&items,this](){emit removeItemsTriggered(items);});
    menu->addAction(removeAction);
    menu->exec(pos);
}
void ZListView::refresh()
{
//    qDebug()<<"call ZListView::refresh()";
    auto tmp=itemsAll;
//    refreshItems(tmp);
}
void ZListView::addItems(QList<DSimpleListItem *> items)
{
    itemsAll.append(items);
    DSimpleListView::addItems(items);
}
void ZListView::clearItems()
{
//    qDebug()<<"call ZListView::clearItems";
    DSimpleListView::clearItems();
}

ZListItem::ZListItem(ZNote *src,QColor c)
{
    note=src;
    color=c;
    updateText();
}
bool ZListItem::sameAs(DSimpleListItem *item)
{
//    qDebug()<<"call ZListItem::sameAs("<<item<<")this="<<this->note;
    return note==(static_cast<ZListItem*>(item))->note;
}
void ZListItem::drawBackground(QRect rect,QPainter *painter,int index,bool isSelect,bool isHover)
{
    QPainterPath path;
    rect.adjust(5,5,-5,-5);
    path.addRoundRect(QRectF(rect),30);
    painter->setOpacity(1);
    if(!isSelect&&!isHover)painter->fillPath(path,Qt::white);
    else if(isSelect)
    {
        painter->fillPath(path,QBrush(QColor(255,247,205)));
        painter->setPen(QPen(color,2));
        painter->drawPath(path);
    }
    else painter->fillPath(path,QColor(255,254,237));
}
void ZListItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover)
{
    QPainterPath path;
    rect.adjust(5,5,-5,-5);
    path.addRect(QRectF(rect));
    painter->setOpacity(1);
    painter->setPen(Qt::black);
    int padding=10;
    QFontMetrics fm(painter->font());
    QString elideText=fm.elidedText(text,Qt::ElideRight,600);
    painter->drawText(QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, elideText);
}
ZNote* ZListItem::data() const
{
    return note;
}
inline void ZListItem::updateText()
{
    text=note->getOverview();
}
QDateTime ZListItem::lastModified() const
{
//    qDebug()<<"call ZListItem::lastModified()"<<note<<note->printObject();
    return note->lastModified();
}
bool ZListItem::sortByDateTime(const DSimpleListItem *a, const DSimpleListItem *b, bool descadingSort)
{
//    qDebug()<<"call sortByDateTime()"<<a<<' '<<b<<endl;
    bool rt=static_cast<const ZListItem*>(a)->lastModified()<static_cast<const ZListItem*>(b)->lastModified();
    return descadingSort?rt:!rt;
}
