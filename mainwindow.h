#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DListView>
#include <DTitlebar>
#include <DSearchEdit>
#include <DTabBar>
#include <QtWidgets>
#include <QDebug>

#include "backend.h"
#include "editor.h"
#include "zlistview.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZBackend *back,QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void save();
private slots:
    void updateOverview(const QString &overview);
    void updateHtml(const QString &html);
    void createNewNote();
    void removeNotes(QList<DSimpleListItem*> items);
private:
    QHBoxLayout *mainLayout;
    ZListView *notesListView;
    Editor *noteEditView;
    ZBackend *backend;
    ZListItem *curNote;
    bool modified;

    void display(DSimpleListItem *item);
    void reset();
    void initNotesListView();
};

#endif // MAINWINDOW_H
