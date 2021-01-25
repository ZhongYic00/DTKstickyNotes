#include "systemtray.h"
#include "daemon.h"

DTK_USE_NAMESPACE
Systemtray::Systemtray(QObject* parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/images/logo256"));

    { // init application
        initMainwindow();
    }

    { // init tray contextMenu
        menu = new QMenu;
        auto addAction = new QAction(tr("新的便笺"), menu);
        auto noteListSubmenu = new QMenu(tr("桌面便笺"));
        auto noteListAction = new QWidgetAction(noteListSubmenu);
        auto noteList = initNoteList(noteListSubmenu);
        noteListAction->setDefaultWidget(noteList);
        auto quitAction = new QAction(tr("退出"), menu);
        connect(addAction, &QAction::triggered, [&]() { Daemon::instance()->addItem(ZNote(false)); });
        connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
        menu->addAction(addAction);
        menu->addMenu(noteListSubmenu);
        noteListSubmenu->addAction(noteListAction);
        menu->addAction(quitAction);
    }

    connect(this, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            if (!win)
                initMainwindow();
            if (win->isHidden())
                win->show();
            else
                win->activateWindow();
        }
        if (reason == QSystemTrayIcon::Context) {
            if (menu) {
                menu->popup(QCursor::pos());
            }
        }
    });
    connect(Daemon::instance(), &Daemon::lostActivation, [&](QWidget* obj) {
        if (obj->isHidden())
            return;
        //        qDebug() << "lostActivation emitted";
        auto stickyNote = qobject_cast<StickyWidget*>(obj);
        assert(stickyNote);
        //        Daemon::instance()->sourceModel()->dbg();
        //        this->commitStickyNoteModifications(stickyNote);
        //        Daemon::instance()->sourceModel()->dbg();
    });
    connect(Daemon::instance(), &Daemon::activateStickyNote, this, &Systemtray::activateStickyNote);
}
void Systemtray::initMainwindow()
{
    win = new MainWindow;
    Dtk::Widget::moveToCenter(win);
    win->show();
    connect(win, &MainWindow::sigDestruct, [this]() { win = nullptr; });
}
QWidget* Systemtray::initNoteList(QWidget* parent)
{
    auto list = new ZListView(parent);
    auto model = new QSortFilterProxyModel(list);

    model->setSourceModel(Daemon::instance()->sourceModel());
    //    model->setDynamicSortFilter(false);
    model->setFilterRole(ZListModel::Attachment);
    model->setFilterFixedString("false");

    connect(model, &QSortFilterProxyModel::rowsInserted, [this, model](const QModelIndex& parent, int first, int last) {
        assert(parent == QModelIndex());
        assert(first == last);
        addNote(model->index(first, 0).data(Qt::UserRole).value<ZNote>());
    });
    connect(model, &QSortFilterProxyModel::rowsAboutToBeRemoved, [this, model](const QModelIndex& parent, int first, int last) {
        assert(parent == QModelIndex());
        assert(first == last);
        removeIdxTmp = model->index(first, 0).data(ZListModel::IndexRole).value<InnerIndex>();
        //        qDebug() << "rowsAboutToBeRemoved emitted" << model->index(first, 0).data(ZListModel::IndexRole); //顺序调整也会经此步骤
        //        removeNote(model->index(first, 0).data(ZListModel::IndexRole).value<InnerIndex>());
    });
    connect(model, &QSortFilterProxyModel::rowsRemoved, [this]() {
        //        qDebug() << "rowsRemoved emitted";
        removeNote(removeIdxTmp);
    });

    list->setNoBackground(true);
    list->setModel(model);
    list->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(list, &ZListView::clicked, [this, list](const QModelIndex& index) {
        //        Daemon::instance()->sourceModel()->dbg();
        //        qDebug() << "request stickyWidget of" << index.data(ZListModel::IndexRole).value<InnerIndex>() << "result=" << stickyWidgets[index.data(ZListModel::IndexRole).value<InnerIndex>()];
        activateStickyNote(index.data(ZListModel::IndexRole).value<InnerIndex>());
        menu->hide();
        list->clearSelection();
    });

    return list;
}
void Systemtray::commitStickyNoteModifications(const InnerIndex& ori, const InnerIndex& idx)
{
    //    qDebug() << "mapping of" << ori << "changed to" << idx << stickyWidgets[ori];
    stickyWidgets[idx] = stickyWidgets[ori];
    stickyWidgets.erase(ori);
    //    widget->commitModifications();
}
void Systemtray::addNote(const ZNote& note)
{
    auto widget = new StickyWidget;
    widget->setNote(note);
    widget->installEventFilter(Daemon::instance());
    stickyWidgets[widget->noteIndex()] = widget;
    connect(widget->textEditor(), &ZTextEdit::indexChanged, this, &Systemtray::commitStickyNoteModifications);
    connect(widget, &StickyWidget::attach, [widget]() {
        widget->removeEventFilter(Daemon::instance());
        //        this->commitStickyNoteModifications(widget);
    });

    widget->show();
}
void Systemtray::removeNote(const InnerIndex& idx)
{
    assert(stickyWidgets.count(idx));
    stickyWidgets.erase(idx);
}
void Systemtray::activateStickyNote(const InnerIndex& idx)
{
    auto widget = stickyWidgets[idx];
    if (!widget->isActiveWindow()) { //optimize needed
        widget->setWindowFlags((widget->windowFlags() & Qt::WindowStaysOnTopHint));
        widget->setWindowFlags((widget->windowFlags() & ~Qt::WindowStaysOnTopHint));
        widget->show();
    }
}
