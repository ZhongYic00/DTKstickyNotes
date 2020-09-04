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
    void addNote(ZNote note=ZNote("detach"), bool existing=false);
private slots:
    void updateNote(StickyWidget *note);
};

#endif // SYSTEMTRAY_H
