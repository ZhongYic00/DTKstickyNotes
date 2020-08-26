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

    connect(notesListView,&ZList::currentChanged,[this](const QModelIndex &item){noteEditView->blockSignals(true);noteEditView->display(item.data(Qt::UserRole).value<ZNote>().getHtml());noteEditView->blockSignals(false);});
    connect(noteEditView,&Editor::contentChanged,[this](const pss val){notesListView->setCurrentOverview(val.first);notesListView->setCurrentHtml(val.second);qDebug()<<"contentChanged";});
    connect(noteEditView,&Editor::contentChanged,[this](){modified=true;});
    connect(notesListView,&ZList::addButtonClicked,this,&MainWindow::createNewNote);
    connect(notesListView,&ZList::listEmptied,[this](){reset();});
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
void MainWindow::save()
{
    if(!modified)return ;
    notesListView->commitChange();
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

