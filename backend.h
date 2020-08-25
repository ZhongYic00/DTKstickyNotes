#ifndef BACKEND_H
#define BACKEND_H
#include <QtCore>
#include "znote.h"

class ZBackend
{
public:
    ZBackend();
    ~ZBackend();
    QList<ZNote> getSavedDataList() const;
    void save(const QList<ZNote> &src);
private:
    QJsonDocument exportAsJson(const QList<ZNote> &src);
    inline QString storageFileName() const;
    inline QString readWholeFile(const QString &filename) const;
};

#endif // BACKEND_H
