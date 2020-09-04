#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DTitlebar>
#include <DSearchEdit>
#include <DTabBar>
#include <QtWidgets>
#include <QDebug>

#include "editor.h"
#include "zlist.h"
#include "searchwidget.h"
#include "stickywidget.h"
#include "daemon.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Daemon *daemon, QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void save();
protected:
    void closeEvent(QCloseEvent *e);
private:
    Daemon *daemon;
    QHBoxLayout *mainLayout;
    ZList *notesListView;
    Editor *noteEditView;
    bool modified;  //records changes to any item

    void display(const QModelIndex &item);
    void reset();
    void initNotesListView();
};

#endif // MAINWINDOW_H
