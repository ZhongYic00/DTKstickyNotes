#include "mainwindow.h"
#include <DApplication>
#include <DThemeManager>
#include <DVerticalLine>

DTK_USE_NAMESPACE

MainWindow::MainWindow(QWidget* parent)
    : DMainWindow(parent)
    , modified(false)
{
    setMinimumSize(800, 800);

    mainLayout = new QHBoxLayout(this);
    notesList = new ZList(this);
    notesList->setObjectName("List");
    notesList->setFixedWidth(300);
    noteEditor = new Editor(this);
    noteEditor->setObjectName("Editor");
    auto spliter = new DVerticalLine(this);
    spliter->setLineWidth(2);
    spliter->setFixedWidth(6);
    mainLayout->addWidget(notesList);
    mainLayout->addWidget(spliter);
    mainLayout->addWidget(noteEditor);
    mainLayout->addSpacing(5);

    connect(notesList, &ZList::currentChanged, [this](const QModelIndex& item) {
        noteEditor->blockSignals(true);
        noteEditor->display(item.data(Qt::UserRole).value<ZNote>().getHtml());
        noteEditor->blockSignals(false);
    });
    connect(noteEditor, &Editor::contentChanged, [this](const pss val) {
        notesList->setCurrentOverview(val.first);
        notesList->setCurrentHtml(val.second);
        modified = true;
    });
    connect(notesList, &ZList::listEmptied, [this]() { reset(); });

    initNotesList();

    titlebar()->setTitle("深度便笺");
    titlebar()->setIcon(QIcon(":/images/logo256"));
    titlebar()->setFixedHeight(40);
    for (auto i : titlebar()->children()) { // adjust titlebar buttons so that they're vertical-centered
        if (qobject_cast<QWidget*>(i))
            qobject_cast<QWidget*>(i)->setFixedHeight(40);
    }
    auto globalSearchBox = new SearchWidget(this);
    globalSearchBox->setObjectName("searchBox");
    titlebar()->setCustomWidget(globalSearchBox, true);
    globalSearchBox->setMaximumWidth(800);
    globalSearchBox->setModel(notesList->getModel());
    connect(globalSearchBox, &SearchWidget::selectItem, notesList, &ZList::setCurrentIndex);

    QWidget* hiddenLayer = new QWidget(this);
    hiddenLayer->setLayout(mainLayout);

    mainLayout->setContentsMargins(0, 5, 0, 0);
    this->setCentralWidget(hiddenLayer);
    QStatusBar* stat = statusBar();
    stat->setObjectName("statusBar");
    stat->setFixedHeight(15);

    auto saveAction = new QAction(tr("&Save"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    addAction(saveAction);
}
MainWindow::~MainWindow()
{
}
void MainWindow::initNotesList()
{
    //如果没有创建过的note，自动新建
    reset();
}
void MainWindow::save()
{
    if (!modified)
        return;
    notesList->commitChange();
    Daemon::instance()->save();
    modified = false;
}
void MainWindow::reset()
{
    noteEditor->reset();
}
void MainWindow::closeEvent(QCloseEvent* e)
{
    e->ignore();
    hide();
}
