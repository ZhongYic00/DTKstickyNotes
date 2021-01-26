#include "mainwindow.h"
#include "daemon.h"
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
    titlebar()->setTitle(tr("便笺"));
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
    globalSearchBox->setModel(Daemon::instance()->sourceModel());
    //    connect(globalSearchBox, &SearchWidget::selectItem, notesList, &ZList::setCurrentIndex);

    QWidget* hiddenLayer = new QWidget(this);
    hiddenLayer->setLayout(mainLayout);

    mainLayout->setContentsMargins(0, 5, 0, 0);
    this->setCentralWidget(hiddenLayer);
    QStatusBar* stat = statusBar();
    stat->setObjectName("statusBar");
    stat->setFixedHeight(15);

    auto saveAction = new QAction(tr("保存"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    addAction(saveAction);

    connect(notesList, &ZList::currentChanged, [this](const QModelIndex& item) {
        noteEditor->setNote(item.data(Qt::UserRole).value<ZNote>());
    });
    connect(notesList, &ZList::listEmptied, [this]() { noteEditor->reset(); });
    connect(noteEditor, &Editor::indexUpdated, notesList, &ZList::setCurrentIndex);
    connect(Daemon::instance(), &Daemon::activateMainwindow, notesList, &ZList::setCurrentIndex);
}
MainWindow::~MainWindow()
{
    qDebug() << "delete Mainwindow";
    emit sigDestruct();
}
void MainWindow::initNotesList()
{
    //如果没有创建过的note，自动新建
}
void MainWindow::save()
{
    if (!modified)
        return;
    //    notesList->commitChange();
    Daemon::instance()->save();
    modified = false;
}
void MainWindow::closeEvent(QCloseEvent* e)
{
    DMainWindow::closeEvent(e);
    deleteLater();
}
