#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#include "mainwindow.h"
#include <DWidgetUtil>
#include <QList>
#include <QMenu>
#include <QSystemTrayIcon>

class Systemtray : public QSystemTrayIcon {
    Q_OBJECT
public:
    Systemtray(QObject* parent = nullptr);
    ~Systemtray()
    {
        if (win)
            delete win;
        delete menu;
    }
    void activateStickyNote(const InnerIndex& idx);

private:
    MainWindow* win;
    QMenu* menu;
    std::map<InnerIndex, StickyWidget*> stickyWidgets;
    InnerIndex removeIdxTmp;

    void initMainwindow();
    QWidget* initNoteList(QWidget* parent);
    void addNote(const ZNote& note = ZNote(false));
    void removeNote(const InnerIndex& idx);
private slots:
    void commitStickyNoteModifications(const InnerIndex& ori, const InnerIndex& idx);
};
#endif // SYSTEMTRAY_H
