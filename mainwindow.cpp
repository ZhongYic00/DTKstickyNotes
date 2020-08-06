#include "mainwindow.h"


MainWindow::MainWindow(ZBackend *back,QWidget *parent) :
    DMainWindow(parent),curNote(nullptr)
{
    backend=back;

    mainLayout=new QHBoxLayout(this);
    notesListView=new ZListView;
    notesListView->setObjectName("ListView");
    noteEditView=new Editor(this);
    noteEditView->setObjectName("Editor");

    connect(notesListView,&ZListView::mousePressChanged,this,&MainWindow::display);
    connect(noteEditView,&Editor::contentChanged,this,&MainWindow::updateOverview);
    connect(notesListView,&ZListView::addButtonClicked,this,&MainWindow::createNewNote);
    connect(notesListView,&ZListView::removeItemsTriggered,this,&MainWindow::removeNotes);
    initNotesListView();

    setMinimumSize(800,800);

    titlebar()->setTitle("深度便签");
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
    auto l=backend->getDataList();
    QList<DSimpleListItem*> items;
    for(auto i:l)
    {
        items.push_back(new ZListItem(i.second,notesListView->borderColor));
    }
    notesListView->addItems(items);
//如果没有创建过的note，自动新建
    reset();
}
void MainWindow::display(DSimpleListItem *item)
{
    auto clickedNote=static_cast<ZListItem*>(item);
    if(curNote==clickedNote)return ;
    if(curNote)updateHtml(noteEditView->getContentRich());
    curNote=clickedNote;
    noteEditView->display(curNote->data()->getHtml());
    notesListView->refresh();
}
void MainWindow::updateOverview(const QString &overview)
{
//    qDebug()<<curNote<<curNote->data()->printObject();
    if(!curNote&&overview.isEmpty())return ;
    else if(!curNote);
    else {
        backend->updateOverview(curNote->lastModified(),overview);
        curNote->updateText();
        notesListView->repaint();
    }
}
void MainWindow::updateHtml(const QString &html)
{
    if(!curNote)return ;
    backend->updateNote(curNote->lastModified(),html);
}
void MainWindow::save()
{
    updateHtml(noteEditView->getContentRich());
    backend->save();
}
void MainWindow::createNewNote()
{
    auto newNote=backend->addNote();
    auto newItem=new ZListItem(newNote,notesListView->borderColor);
    notesListView->addItems({newItem});
    notesListView->selectLastItem();
    display(newItem);
}
void MainWindow::removeNotes(QList<DSimpleListItem*> items)
{
    for(auto i:items)
    {
        auto item=static_cast<ZListItem*>(i);
        backend->removeNote(item->data());
        notesListView->removeItem(item);
    }
    notesListView->selectNextItem();
    reset();
}
void MainWindow::reset()
{
    curNote=nullptr;
    noteEditView->reset();
    notesListView->clearSelections();
}
MainWindow::~MainWindow()
{
}

