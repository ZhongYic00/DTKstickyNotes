#include "mainwindow.h"
#include <DApplication>
#include <DThemeManager>
#include <DVerticalLine>

DTK_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) : DMainWindow(parent), modified(false) {
	setMinimumSize(800, 800);

	mainLayout = new QHBoxLayout(this);
	notesListView = new ZList(this);
	notesListView->setObjectName("List");
	notesListView->setFixedWidth(300);
	noteEditView = new Editor(this);
	noteEditView->setObjectName("Editor");
	auto spliter = new DVerticalLine(this);
	spliter->setLineWidth(2);
	spliter->setFixedWidth(6);
	mainLayout->addWidget(notesListView);
	mainLayout->addWidget(spliter);
	mainLayout->addWidget(noteEditView);
	mainLayout->addSpacing(5);

	connect(notesListView, &ZList::currentChanged, [this](const QModelIndex &item) {
		noteEditView->blockSignals(true);
		noteEditView->display(item.data(Qt::UserRole).value<ZNote>().getHtml());
		noteEditView->blockSignals(false);
	});
	connect(noteEditView, &Editor::contentChanged, [this](const pss val) {
		notesListView->setCurrentOverview(val.first);
		notesListView->setCurrentHtml(val.second);
		modified = true;
	});
	connect(notesListView, &ZList::listEmptied, [this]() { reset(); });

	initNotesListView();

	titlebar()->setTitle("深度便笺");
	titlebar()->setIcon(QIcon(":/images/logo256"));
	titlebar()->setFixedHeight(40);
	// adjust titlebar buttons so that they're vertical-centered
	titlebar()->findChild<QWidget *>("DTitlebarDWindowMaxButton")->setFixedHeight(40);
	titlebar()->findChild<QWidget *>("DTitlebarDWindowMinButton")->setFixedHeight(40);
	titlebar()->findChild<QWidget *>("DTitlebarDWindowCloseButton")->setFixedHeight(40);
	titlebar()->findChild<QWidget *>("DTitlebarDWindowOptionButton")->setFixedHeight(40);
	auto *globalSearchBox = new SearchWidget(titlebar());
	globalSearchBox->setObjectName("searchBox");
	//    globalSearchBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
	globalSearchBox->setMaximumWidth(800);
	globalSearchBox->setSourceModel(notesListView->getModel());
	titlebar()->setCustomWidget(globalSearchBox, true);
	connect(globalSearchBox, &SearchWidget::changeCurrent, notesListView, &ZList::setCurrentIndex);

	QWidget *hiddenLayer = new QWidget(this);
	hiddenLayer->setLayout(mainLayout);

	mainLayout->setContentsMargins(0, 5, 0, 0);
	this->setCentralWidget(hiddenLayer);
	QStatusBar *stat = statusBar();
	stat->setObjectName("statusBar");
	stat->setFixedHeight(15);

	auto saveAction = new QAction(tr("&Save"));
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered, this, &MainWindow::save);
	addAction(saveAction);
}
MainWindow::~MainWindow() {
}
void MainWindow::initNotesListView() {
	//如果没有创建过的note，自动新建
	reset();
}
void MainWindow::save() {
	if (!modified)
		return;
	notesListView->commitChange();
	Daemon::instance()->save();
	modified = false;
}
void MainWindow::reset() {
	noteEditView->reset();
}
void MainWindow::closeEvent(QCloseEvent *e) {
	e->ignore();
	hide();
}
