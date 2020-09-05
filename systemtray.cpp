#include "systemtray.h"

DTK_USE_NAMESPACE
Systemtray::Systemtray(QObject *parent) : QSystemTrayIcon(parent) {
	/*    auto addNote=[&](ZNote note=ZNote("detach"),bool existing=false){
			if(!existing)daemon->addItem(note);
			qDebug()<<"addNote(";note.print();
			std::cerr<<this;
			auto widget=new StickyWidget(this);
			widget->setNote(note);
			updateNote(widget);
			connect(widget,&StickyWidget::attach,[this,widget](){
				widget->removeEventFilter(daemon);
				this->updateNote(widget);
				qDebug()<<"just a signal";
			});
			widget->installEventFilter(daemon);
			widget->show();
		};*/

	setIcon(QIcon(":/images/logo256"));

	{ // init application
		daemon = new Daemon(this);
		win = new MainWindow(daemon);
		Dtk::Widget::moveToCenter(win);
		win->show();
	}

	{ // init stickyNotes list
		stickyNotes = new QSortFilterProxyModel(this);
		stickyNotes->setSourceModel(daemon->getModel());
		stickyNotes->setFilterRole(ZListModel::DisplayType);
		stickyNotes->setFilterFixedString("detach");
	}

	{ // init contextMenu
		auto menu = new QMenu;
		auto add = new QAction(tr("Add"), menu);
		noteSubmenu = new QMenu(tr("sticky notes"));
		auto quit = new QAction(tr("Quit"), menu);
		connect(add, &QAction::triggered, [&]() { addNote(); });
		connect(quit, &QAction::triggered, qApp, &QApplication::quit);
		menu->addAction(add);
		menu->addMenu(noteSubmenu);
		menu->addAction(quit);
		setContextMenu(menu);
	}

	connect(this, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::Context)
			;
		else if (reason == QSystemTrayIcon::Trigger) {
			if (win->isHidden())
				win->show();
			else
				win->activateWindow();
		}
	});
	connect(daemon, &Daemon::lostActivation, [&](QWidget *obj) {
		if (obj->isHidden())
			return;
		auto stickyNote = qobject_cast<StickyWidget *>(obj);
		this->updateNote(stickyNote);
	});
	connect(daemon, &Daemon::itemDetached, [&](InnerIndex idx) {
		/*        auto subIdx=stickyNotes->mapFromSource(index);
				if(subIdx.isValid()){
					qDebug()<<"subIndex is Valid";
					addNote(index.data(Qt::UserRole).value<ZNote>(),true);
				}*/
		addNote(daemon->getModel()->data(idx, Qt::UserRole).value<ZNote>(), true);
	});
}
void Systemtray::updateNote(StickyWidget *obj) {
	auto note = obj->getNote();
	//    qDebug()<<this<<"updateNote";note.print();
	auto idx = note.getUpdateTimeRaw();
	daemon->getModel()->indexOf(idx);
	daemon->setHtml(idx, note.getHtml());
	daemon->setOverview(idx, note.getOverview());
	daemon->commitChange(idx, note.getAttach() == "attach");
	note = stickyNotes->data(stickyNotes->index(0, 0), Qt::UserRole).value<ZNote>();
	obj->setNote(note);
}
void Systemtray::save() {
	daemon->save();
}
void Systemtray::addNote(ZNote note, bool existing) {
	if (!existing)
		daemon->addItem(note);
	//        qDebug()<<"addNote(";note.print();
	auto widget = new StickyWidget(this);
	widget->setNote(note);
	updateNote(widget);
	connect(widget, &StickyWidget::attach, [this, widget]() {
		widget->removeEventFilter(daemon);
		this->updateNote(widget);
		this->updateStickyNotesMenu(widget, false);
		//        delete widget;
	});
	widget->installEventFilter(daemon);
	updateStickyNotesMenu(widget, true);
	widget->show();
}
void Systemtray::updateStickyNotesMenu(StickyWidget *widget, bool isAdding) {
	qDebug() << "call updateStickyNotesMenu";
	auto generateAction = [&](StickyWidget *widget) {
		qDebug() << "generateAction";
		auto rt = new QAction(noteSubmenu);
		auto elide = [&](const QString &text) {
			QFontMetrics m(rt->font());
			return m.elidedText(text.left(50), Qt::ElideRight, 100);
		};
		rt->setText(elide(widget->getNote().getOverview()));
		connect(rt, &QAction::triggered, [widget]() { widget->activateWindow(); });
		connect(widget, &StickyWidget::textChanged, [rt](const QString &text) {
			QFontMetrics m(rt->font());
			rt->setText(m.elidedText(text.left(50), Qt::ElideRight, 100));
		});
		return rt;
	};
	auto refreshContextMenu = [&]() {
		contextMenu()->clear();
		auto add = new QAction(tr("Add"), contextMenu());
		auto quit = new QAction(tr("Quit"), contextMenu());
		connect(add, &QAction::triggered, [&]() { addNote(); });
		connect(quit, &QAction::triggered, qApp, &QApplication::quit);
		contextMenu()->addAction(add);
		contextMenu()->addMenu(noteSubmenu);
		contextMenu()->addAction(quit);
	};
	if (isAdding) {
		qDebug() << "isAdding" << widget->getNote().getOverview();
		auto action = generateAction(widget);
		stickyWidgets[widget] = action;
		noteSubmenu->addAction(action);
		refreshContextMenu();
	} else {
		auto action = stickyWidgets[widget];
		noteSubmenu->removeAction(action);
		stickyWidgets.erase(widget);
		refreshContextMenu();
		action->deleteLater();
	}
}
