#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#include <QSystemTrayIcon>
#include <QMenu>
#include <QList>
#include <DWidgetUtil>
#include "mainwindow.h"

class Systemtray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    Systemtray(QObject *parent=nullptr);
    void save();
private:
    MainWindow *win;
    Daemon *daemon;
    QSortFilterProxyModel *stickyNotes;
    QMenu *noteSubmenu;
    std::map<StickyWidget*,QAction*> stickyWidgets;
    void addNote(ZNote note=ZNote("detach"), bool existing=false);
    void updateStickyNotesMenu(StickyWidget *widget, bool isAdding);
private slots:
    void updateNote(StickyWidget *note);
};

#endif // SYSTEMTRAY_H
