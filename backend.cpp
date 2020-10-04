#include "backend.h"

ZBackend::ZBackend() : mediaSourceId(0) {
	initPath();
	initMedia();
}
ZBackend::~ZBackend() {
	qDebug() << "call ZBackend::~ZBackend()";
	saveMediaFile();
}
void ZBackend::initPath() { // now just implemented mainfile
	qDebug() << "call ZBackend::initPath";
	bool chk = true;
	for (auto i : { MainFile, MediaSourceFile, ConfigFile }) {
		QDir dir(storagePath(i));
		if (!dir.exists()) {
			qDebug() << "Path" << i << "does not exists, init now";
			dir.mkpath(storagePath(i));
			chk = false;
		}
	}
	if (chk)
		return;
	QFile oldVersion("stickyNotes.json");
	if (oldVersion.exists()) {
		qDebug() << "oldVersion detected at" << QDir::currentPath() + '/' + oldVersion.fileName() << ", transform now";
		oldVersion.rename(storageFileFullName());
	}
}
void ZBackend::initMedia() {
	qDebug() << "call ZBackend::initMedia()";
	qDebug() << readWholeFile(storageFileFullName(MediaFile));
	QFile list(storageFileFullName(MediaFile));
	if (!list.open(QFile::ReadOnly | QFile::Text))
		return;
	//	qDebug() << list.readLine().simplified().toInt();
	mediaSourceId = list.readLine().simplified().toInt();
	while (!list.atEnd()) {
		auto line = list.readLine().simplified().split(' ');
		qDebug() << line;
		media[QByteArray::fromBase64(line[0])] = QString::fromLocal8Bit(line[1]);
	}
	qDebug() << media.size() << "records read";
	//	qDebug() << calcFileMd5("/home/zyc/Pictures/Wallpapers/灵笼/8ae42dc968d807969365c4f0607275afa4439057.png").toBase64();
	list.close();
}
void ZBackend::saveMediaFile() {
	QFile output(storageFileFullName(MediaFile));
	output.open(QFile::WriteOnly | QFile::Text);
	QTextStream outputStream(&output);
	outputStream << mediaSourceId << endl;
	for (auto i = media.begin(); i != media.end(); i++) {
		outputStream << QString::fromLocal8Bit(i.key().toBase64()) << ' ' << i.value() << endl;
	}
	output.close();
}
void ZBackend::saveMainFile(const QList<ZNote> &src) { //下一步实现双文件储存，防数据遗失
	qDebug() << "saving mainfile...";
	QFile outputTemp(storagePath() + '~' + storageFileName());
	//	QFile output(storageFileFullName());
	outputTemp.open(QFile::WriteOnly | QFile::Text);
	QTextStream outputStream(&outputTemp);
	outputStream << exportAsJson(src).toJson(QJsonDocument::Compact);
	outputTemp.close();
	QFile::remove(storageFileFullName());
	QFile::copy(storagePath() + '~' + storageFileName(), storageFileFullName());
}
inline int ZBackend::generateFileID() {
	return ++mediaSourceId;
}
QString ZBackend::insertImage(const QUrl &fileUrl) {
	if (fileUrl.isLocalFile()) {
		auto hash = calcFileMd5(fileUrl.path());
		if (!media.contains(hash)) {
			qDebug() << "file" << fileUrl << "not found in hashTable, now insert";
			auto filename = QString::number(generateFileID());
			qDebug() << "now copy" << fileUrl.path() << "to" << storagePath(MediaSourceFile) + filename;
			QFile::copy(fileUrl.path(), storagePath(MediaSourceFile) + filename);
			media[hash] = filename;
		}
		return hash.toBase64();
	} else {
	}
	return "";
}
QString ZBackend::insertImage(const QByteArray &data) {
	auto hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
	if (!media.contains(hash)) {
		auto filename = QString::number(generateFileID());
		QFile file(storagePath(MediaSourceFile) + filename);
		file.open(QFile::WriteOnly);
		file.write(data);
		file.close();
		media[hash] = filename;
	}
	return hash.toBase64();
}
QByteArray ZBackend::queryImage(const QString &hash) {
	if (media.contains(QByteArray::fromBase64(hash.toLocal8Bit()))) {
		auto filename = media[QByteArray::fromBase64(hash.toLocal8Bit())];
		QFile file(storagePath(MediaSourceFile) + filename);
		file.open(QFile::ReadOnly);
		return file.readAll();
	} else {
		return QByteArray();
	}
}
QList<ZNote> ZBackend::getSavedDataList() const {
	qDebug() << storageFileFullName();
	auto objList = QVariant(QJsonDocument::fromJson(readWholeFile(storageFileFullName()).toUtf8()).array().toVariantList()).toList();
	QList<ZNote> rt;
	for (auto i : objList) {
		auto obj = i.toJsonObject();
		rt.push_back(ZNote(obj));
	}
	return rt;
}
inline QString ZBackend::storageFileFullName(int type) {
	return storagePath(type) + storageFileName(type);
}
inline QString ZBackend::storageFileName(int type) {
	switch (type) {
		case MainFile:
			return "stickyNotes.json";
		case BackupFile:
			return "~stickyNotes.json";
		case ConfigFile:
			return "stickyNotes.conf";
		case MediaFile:
			return "stickyNotes.map";
		default:
			return "";
	}
}
inline QString ZBackend::storagePath(int type) {
	switch (type) {
		case BackupFile:
		case MainFile:
		case MediaFile:
			return QString("%1/").arg(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
		case MediaSourceFile:
			return QString("%1/resources/").arg(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
		case ConfigFile:
			return QString("%1/%2/%3/")
			.arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation), qApp->organizationName(), qApp->applicationName());
		default:
			return "";
	}
}
inline QString ZBackend::readWholeFile(const QString &filename) {
	QFile input(filename);
	input.open(QFile::ReadOnly | QFile::Text);
	if (!input.isOpen())
		return QString();
	QTextStream inputStream(&input);
	QString rt = inputStream.readAll();
	//    qDebug()<<"import file content:\n"<<rt;
	input.close();
	return rt;
}
QJsonDocument ZBackend::exportAsJson(const QList<ZNote> &src) {
	QJsonArray array;
	for (auto i : src)
		array.append(QJsonValue(i.jsonObject()));
	return QJsonDocument(array);
}
inline QByteArray ZBackend::calcFileMd5(const QString &filename) {
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	return QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
}
