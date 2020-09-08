#ifndef DAEMON_H
#define DAEMON_H

#include "backend.h"
#include "zlistmodel.h"
#include "zlistview.h"
#include <QObject>

class Daemon : public QObject {
	Q_OBJECT
	public:
	Daemon(QObject *parent = nullptr);
	~Daemon() override;
	void addItem(const ZNote &item);
	void addItems(const QList<ZNote> &items);
	void removeItem(const ZNote &item);
	void removeItems(const QList<ZNote> &items);
	void setHtml(const QDateTime &idx, const QString &html);
	void setOverview(const QDateTime &idx, const QString &overview);
	void commitChange(const QModelIndex &index, bool toggleAttach);
	QDateTime commitChange(const QDateTime &idx, bool toggleAttach);
	ZListModel *getModel();
	QList<ZNote> getDataList() const;
	QList<QPair<QString, QString>> getResourceList() const;
	void save();
	QObject *systemTray();
	void attach(InnerIndex idx);
	void detach(InnerIndex idx);
	QString calcImageHash(const QUrl &fileUrl);
	QString calcImageHash(const QByteArray &data);
	QByteArray fetchImageData(const QString &hash);

	static Daemon *instance();
	signals:
	void activationChanged(QObject *obj);
	void gainActivation(QWidget *obj);
	void lostActivation(QWidget *obj);
	void itemDetached(InnerIndex idx);

	protected:
	static Daemon *daemon;

	bool eventFilter(QObject *obj, QEvent *e) override;

	private:
	ZBackend *back;
	ZListModel *model;
};

#endif // DAEMON_H
