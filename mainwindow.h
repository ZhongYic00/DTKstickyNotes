#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DSearchEdit>
#include <DTabBar>
#include <DTitlebar>
#include <QDebug>
#include <QtWidgets>

#include "editor.h"
#include "searchwidget.h"
#include "stickywidget.h"
#include "zlist.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
signals:
    void sigDestruct();
public slots:
    void save();

protected:
    void closeEvent(QCloseEvent* e) override;

private:
    QHBoxLayout* mainLayout;
    ZList* notesList;
    Editor* noteEditor;
    bool modified; // records changes to any item

    void initNotesList();
};

#endif // MAINWINDOW_H
