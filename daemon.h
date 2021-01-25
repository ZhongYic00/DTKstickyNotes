#ifndef DAEMON_H
#define DAEMON_H

#include "backend.h"
#include "systemtray.h"
#include "zlistmodel.h"
#include "zlistview.h"
#include <QObject>

class Daemon : public QObject {
    Q_OBJECT
public:
    Daemon(QObject* parent = nullptr);
    ~Daemon() override;
    inline void addItem(const ZNote& item);
    inline void addItems(const QList<ZNote>& items);
    inline void removeItem(const ZNote& item);
    inline void removeItems(const QList<ZNote>& items);
    inline ZListModel* sourceModel();
    inline Systemtray* systemTray();
    inline void toggleAttach(const InnerIndex& idx); //toggle attachment attribute of note, and update updateTime at the same time
    inline void updateNoteContent(const InnerIndex& idx, const QTextDocumentFragment& doc);
    void setHtml(const InnerIndex& idx, const QString& html);
    void setAbstract(const InnerIndex& idx, const QString& Abstract);
    void commitChange(const QModelIndex& index);
    InnerIndex commitChange(const InnerIndex& idx);
    QList<ZNote> exportNotes() const;
    QList<QPair<QString, QString>> getResourceList() const;
    void save();
    QString calcImageHash(const QUrl& fileUrl);
    QString calcImageHash(const QByteArray& data);
    QByteArray fetchImageData(const QString& hash);
    QByteArray fetchRemoteResource(const QString& url);

    static Daemon* instance();
signals:
    void activationChanged(QObject* obj);
    void gainActivation(QWidget* obj);
    void lostActivation(QWidget* obj);
    void activateStickyNote(const InnerIndex& idx);
    void activateMainwindow(const InnerIndex& idx);

protected:
    static Daemon* daemon;

    bool eventFilter(QObject* obj, QEvent* e) override;

private:
    ZBackend* back;
    ZListModel* model;
    Systemtray* tray;
};
void Daemon::addItem(const ZNote& item) { model->appendRow(item); }
void Daemon::addItems(const QList<ZNote>& items)
{
    for (auto i : items)
        addItem(i);
}
void Daemon::removeItem(const ZNote& item) { model->removeRow(item); }
void Daemon::removeItems(const QList<ZNote>& items)
{
    for (auto i : items)
        removeItem(i);
}
ZListModel* Daemon::sourceModel() { return model; }
Systemtray* Daemon::systemTray() { return tray; }
void Daemon::toggleAttach(const InnerIndex& idx)
{
    //    commitChange(idx);
    model->setData(idx, QVariant(), ZListModel::Attachment);
}
void Daemon::updateNoteContent(const InnerIndex& idx, const QTextDocumentFragment& doc) { model->setData(idx, QVariant::fromValue(doc), ZListModel::Content); }
#endif // DAEMON_H
