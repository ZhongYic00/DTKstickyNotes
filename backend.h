#ifndef BACKEND_H
#define BACKEND_H
#include "znote.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtConcurrent/QtConcurrent>
#include <QtCore>

class ZBackend {
	public:
	enum Files { MainFile, BackupFile, ConfigFile, MediaFile, MediaSourceFile };
	ZBackend();
	~ZBackend();
	QList<ZNote> getSavedDataList() const;
	void saveMediaFile();
	void saveMainFile(const QList<ZNote> &src);
	QString insertImage(const QUrl &file);
	QString insertImage(const QByteArray &data);
	QByteArray queryImage(const QString &hash);
	QByteArray download(const QString &url);

	private:
	int mediaSourceId;
	QHash<QByteArray, QString> media; // key refers to hash value of the file, value refers to relative path of media source file
	QNetworkAccessManager *netManager;

	void initMedia();
	inline int generateFileID();

	static inline void initPath();
	static inline QByteArray calcFileMd5(const QString &filename);
	static inline QString readWholeFile(const QString &filename);
	static inline QJsonDocument exportAsJson(const QList<ZNote> &src);
	static inline QString storageFileName(int type = MainFile);
	static inline QString storageFileFullName(int type = MainFile);
	static inline QString storagePath(int type = MainFile);
};

#endif // BACKEND_H
