#include "zlist.h"
#include "editor.h"

DGUI_USE_NAMESPACE
ZList::ZList(QWidget *parent) : QWidget(parent), haveChange(false) {
	//    qDebug()<<curIdx;
	listview = new ZListView(this);
	model = new QSortFilterProxyModel(this);
	model->setSourceModel(Daemon::instance()->getModel());
	model->setFilterRole(ZListModel::DisplayType);
	model->setFilterFixedString("attach");

	listview->setModel(model);
	//    listview->setObjectName("listview");
	listview->setNoBackground(true);

	DGuiApplicationHelper *guiAppHelp = DGuiApplicationHelper::instance();
	themeColor = guiAppHelp->systemTheme()->activeColor();
	setLayout(initAddLayer());

	listview->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(listview, &QListView::customContextMenuRequested, [this](const QPoint &pos) { popupMenu(mapToGlobal(pos)); });
	connect(listview, &ZListView::curIndexChanged, this, &ZList::currentChanged);
	connect(this, &ZList::currentChanged,
			[this](QModelIndex index) { this->curIdx = index.data(Qt::UserRole).value<ZNote>().getUpdateTimeRaw(); });
	connect(listview, &ZListView::listEmptied, [this]() { emit listEmptied(); });
}
QWidget *ZList::initAddButton() {
	auto *layer = new TransparentWidget(this);
	layer->setObjectName("toolBarLayer");

	QVBoxLayout *layout = new QVBoxLayout(layer);
	layer->setLayout(layout);
	layout->addStretch();

	QHBoxLayout *bar = new QHBoxLayout(layer);
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

	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(btn);
	effect->setOffset(0, 0);
	effect->setColor(QColor(0, 0, 0, 150));
	effect->setBlurRadius(10);
	btn->setGraphicsEffect(effect);

	connect(btn, &DPushButton::clicked, [this]() { addItem(ZNote()); });

	bar->addStretch();
	bar->addWidget(btn);
	return layer;
}
QLayout *ZList::initAddLayer() {
	QStackedLayout *layout = new QStackedLayout(this);
	layout->setStackingMode(QStackedLayout::StackAll);
	layout->addWidget(initAddButton());
	layout->addWidget(listview);
	return layout;
}
void ZList::addItem(const ZNote &item) {
	//    qDebug()<<"model->rowCount()"<<model->rowCount();
	Daemon::instance()->addItem(item);
	auto i = model->index(0, 0);
	//    qDebug()<<"model->rowCount()"<<model->rowCount();
	listview->setCurrentIndex(i);
	emit currentChanged(i);
}
void ZList::removeItems(const QList<ZNote> &items) {
	Daemon::instance()->removeItems(items);
}
void ZList::setCurrentOverview(const QString &overview) {
	haveChange = true;
	auto curIndex = listview->currentIndex();
	model->setData(curIndex, QVariant::fromValue(overview), ZListModel::Overview);
}
void ZList::setCurrentHtml(const QString &html) {
	haveChange = true;
	auto curIndex = listview->currentIndex();
	model->setData(curIndex, QVariant::fromValue(html), ZListModel::Html);
}
void ZList::commitChange(bool trace) //优化逻辑，若通过save主动commit，后续setIndex时可能再次触发commit
{
	haveChange = false;
	curIdx = Daemon::instance()->commitChange(curIdx, false);
	if (trace)
		listview->setCurrentIndex(model->index(0, 0));
}
void ZList::popupMenu(const QPoint &pos) {
	auto selection = listview->selectionNotes();
	QMenu *menu = new QMenu();
	QAction *removeAction = new QAction(QIcon(":/images/trash-empty"), tr("删除"), menu);
	auto indexOf = [=](InnerIndex &idx) { return this->model->mapFromSource(Daemon::instance()->getModel()->indexOf(idx)); };
	connect(removeAction, &QAction::triggered, [&selection, this, &indexOf]() {
		listview->clearSelectionExt();
		removeItems(selection);
		listview->setCurrentIndex(indexOf(curIdx));
	});
	menu->addAction(removeAction);
	if (selection.length() == 1) {
		QAction *detachAction = new QAction(tr("解除吸附"), menu);
		auto index = listview->selection()[0];
		connect(detachAction, &QAction::triggered, [index, this, &indexOf]() {
			listview->clearSelectionExt();
			Daemon::instance()->detach(index.data(ZListModel::UpdateTime).value<QDateTime>());
			listview->setCurrentIndex(indexOf(curIdx));
		});
		menu->addAction(detachAction);
	}
	menu->exec(pos);
}
void ZList::setCurrentIndex(const QModelIndex &cur) {
	if (cur.isValid())
		listview->setCurrentIndex(cur);
}
QAbstractItemModel *ZList::getModel() {
	return model;
}
