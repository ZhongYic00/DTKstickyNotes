#include "zlist.h"
#include "daemon.h"
#include "editor.h"
#include <QAbstractItemModelTester>

DGUI_USE_NAMESPACE
ZList::ZList(QWidget* parent)
    : QWidget(parent)
{
    //    qDebug()<<curIdx;
    listview = new ZListView(this);
    model = new QSortFilterProxyModel(this);
    model->setSourceModel(Daemon::instance()->sourceModel());
    model->setFilterRole(ZListModel::Attachment);
    model->setFilterFixedString("true");
    //    model->setDynamicSortFilter(false);

    //    QLoggingCategory::setFilterRules("qt.modeltest.debug=true");
    //    new QAbstractItemModelTester(model, QAbstractItemModelTester::FailureReportingMode::Fatal, this);

    listview->setModel(model);
    listview->setNoBackground(true);

    DGuiApplicationHelper* guiAppHelp = DGuiApplicationHelper::instance();
    themeColor = guiAppHelp->systemTheme()->activeColor();
    setLayout(initAddLayer());

    listview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listview, &QListView::customContextMenuRequested, [this](const QPoint& pos) { popupMenu(mapToGlobal(pos)); });
    connect(listview, &ZListView::currentIndexChanged, this, &ZList::currentChanged);
    connect(listview, &ZListView::listEmptied, [this]() { emit listEmptied(); });
}
QWidget* ZList::initAddButton()
{
    auto* layer = new TransparentWidget(this);
    layer->setObjectName("toolBarLayer");

    QVBoxLayout* layout = new QVBoxLayout(layer);
    layer->setLayout(layout);
    layout->addStretch();

    QHBoxLayout* bar = new QHBoxLayout(layer);
    layout->addLayout(bar);
    layout->addSpacing(10);

    QIcon ico(":/images/add-btn");
    auto btn = new DPushButton(ico, "", this);
    btn->setObjectName("listAddButton");
    btn->setIconSize(QSize(50, 50));
    btn->resize(50, 50);
    btn->setStyleSheet(QString("#listAddButton{"
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
                               "}")
                           .arg(themeColor.name(QColor::HexRgb), themeColor.darker(110).name(QColor::HexRgb),
                               themeColor.darker(150).name(QColor::HexRgb)));

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(btn);
    effect->setOffset(0, 0);
    effect->setColor(QColor(0, 0, 0, 150));
    effect->setBlurRadius(10);
    btn->setGraphicsEffect(effect);

    connect(btn, &DPushButton::clicked, [this]() { addItem(ZNote()); });

    bar->addStretch();
    bar->addWidget(btn);
    return layer;
}
QLayout* ZList::initAddLayer()
{
    QStackedLayout* layout = new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->addWidget(initAddButton());
    layout->addWidget(listview);
    return layout;
}
void ZList::addItem(const ZNote& item)
{
    qDebug() << "model->rowCount()" << model->rowCount();
    Daemon::instance()->addItem(item);
    auto i = model->index(0, 0);
    qDebug() << "model->rowCount()" << model->rowCount();
    for (auto i = 0; i < model->rowCount(); i++) {
        qDebug() << model->data(model->index(i, 0), ZListModel::Attachment);
    }
    listview->setCurrentIndex(i);
    emit currentChanged(i);
}
void ZList::removeItems(const QList<ZNote>& items)
{
    Daemon::instance()->removeItems(items);
}
void ZList::popupMenu(const QPoint& pos)
{
    auto selection = listview->selectionNotes();
    QMenu* menu = new QMenu();
    QAction* removeAction = new QAction(QIcon(":/images/trash-empty"), tr("删除"), menu);
    connect(removeAction, &QAction::triggered, [&selection, this]() {
        listview->clearSelectionExt();
        removeItems(selection);
        //        listview->setCurrentIndex(indexOf(curIdx));
    });
    menu->addAction(removeAction);
    if (selection.length() == 1) {
        QAction* detachAction = new QAction(tr("解除吸附"), menu);
        auto index = listview->selection()[0];
        connect(detachAction, &QAction::triggered, [index, this]() {
            listview->clearSelectionExt();
            Daemon::instance()->sourceModel()->dbg();
            qDebug() << "toggle attachment of" << index.data(ZListModel::IndexRole).value<InnerIndex>();
            Daemon::instance()->toggleAttach(index.data(ZListModel::IndexRole).value<InnerIndex>());
            Daemon::instance()->sourceModel()->dbg();
            //            listview->setCurrentIndex(indexOf(curIdx));
        });
        menu->addAction(detachAction);
    }
    menu->exec(pos);
}
void ZList::setCurrentIndex(const InnerIndex& idx) { listview->setCurrentIndex(model->mapFromSource(Daemon::instance()->sourceModel()->indexOf(idx))); }
QAbstractItemModel* ZList::sourceModel()
{
    return model;
}
