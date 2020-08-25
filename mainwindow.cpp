#include "mainwindow.h"


MainWindow::MainWindow(ZBackend *back,QWidget *parent) :
    DMainWindow(parent),modified(false)
{
    backend=back;

    mainLayout=new QHBoxLayout(this);
    notesListView=new ZList;
    notesListView->setObjectName("List");
    noteEditView=new Editor(this);
    noteEditView->setObjectName("Editor");

    connect(notesListView,&ZList::currentChanged,this,&MainWindow::display);
    connect(noteEditView,&Editor::contentChanged,this,&MainWindow::updateOverview);
    connect(noteEditView,&Editor::contentChanged,[this](){modified=true;});
    connect(notesListView,&ZList::addButtonClicked,this,&MainWindow::createNewNote);
//    connect(notesListView,&ZListView::removeItemsTriggered,this,&MainWindow::removeNotes);
    initNotesListView();

    setMinimumSize(800,800);

    titlebar()->setTitle("深度便笺");
    titlebar()->setIcon(QIcon(":/images/logo256"));
    titlebar()->setFixedHeight(40);
    QWidget *globalSearchBox=new DSearchEdit(titlebar());
    globalSearchBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    globalSearchBox->setMaximumWidth(800);
    titlebar()->setCustomWidget(globalSearchBox,true);

    mainLayout->addWidget(notesListView);
    mainLayout->addWidget(noteEditView);

    QWidget *hiddenLayer=new QWidget(this);
    hiddenLayer->setLayout(mainLayout);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setCentralWidget(hiddenLayer);
    QStatusBar *stat=statusBar();
    stat->setObjectName("statusBar");

    auto saveAction=new QAction(tr("&Save"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction,&QAction::triggered,this,&MainWindow::save);
    addAction(saveAction);
}
void MainWindow::initNotesListView()
{
    auto list=backend->getSavedDataList();
    notesListView->addItems(list);
//如果没有创建过的note，自动新建
    reset();
}
void MainWindow::display(const QModelIndex &item)
{
    noteEditView->display(item.data(Qt::UserRole).value<ZNote>().getHtml());
}
void MainWindow::updateOverview(const QString &overview)
{
    notesListView->setCurrentOverview(overview);
}
void MainWindow::updateHtml(const QString &html)
{
    notesListView->setCurrentHtml(html);
}
void MainWindow::save()
{
    if(!modified)return ;
    updateHtml(noteEditView->getContentRich());
    backend->save(notesListView->getDataList());
    modified=false;
}
void MainWindow::createNewNote()
{
}
void MainWindow::reset()
{
    noteEditView->reset();
}
MainWindow::~MainWindow()
{
}

