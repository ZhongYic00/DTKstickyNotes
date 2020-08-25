#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DTitlebar>
#include <DSearchEdit>
#include <DTabBar>
#include <QtWidgets>
#include <QDebug>

#include "backend.h"
#include "editor.h"
#include "zlist.h"

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
private:
    QHBoxLayout *mainLayout;
    ZList *notesListView;
    Editor *noteEditView;
    ZBackend *backend;
    bool modified;

    void display(const QModelIndex &item);
    void reset();
    void initNotesListView();
};

#endif // MAINWINDOW_H
