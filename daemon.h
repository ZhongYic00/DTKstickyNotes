#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include "backend.h"
#include "zlistview.h"
#include "zlistmodel.h"

class Daemon : public QObject
{
    Q_OBJECT
public:
    Daemon(QObject *parent=nullptr);
    void addItem(const ZNote &item);
    void addItems(const QList<ZNote> &items);
    void removeItem(const ZNote &item);
    void removeItems(const QList<ZNote> &items);
    QList<ZNote> getDataList() const;
    void setHtml(const QDateTime &idx, const QString &html);
    void setOverview(const QDateTime &idx, const QString &overview);
    void commitChange(const QModelIndex &index, bool toggleAttach);
    QDateTime commitChange(const QDateTime &idx, bool toggleAttach);
    ZListModel* getModel();
    void save();
    QObject* systemTray();
    void attach(InnerIndex idx);
    void detach(InnerIndex idx);
signals:
    void activationChanged(QObject *obj);
    void gainActivation(QWidget *obj);
    void lostActivation(QWidget *obj);
    void itemDetached(InnerIndex idx);
protected:
    bool eventFilter(QObject *obj, QEvent *e) override;
private:
    ZBackend *back;
    ZListModel *model;
};

#endif // DAEMON_H
