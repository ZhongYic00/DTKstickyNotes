#ifndef BACKEND_H
#define BACKEND_H
#include <QtCore>
#include "znote.h"

class ZBackend
{
public:
    ZBackend();
    ~ZBackend();
    ZNote* addNote();
    void removeNote(ZNote *note);
    void updateOverview(QDateTime lastModified,QString plain);
    void updateNote(QDateTime lastModified,QString rich);
    void save();
    std::vector<std::pair<QDateTime,ZNote*> > getDataList()const;
private:
    std::map<QDateTime,ZNote*> data;
    QFile tmpFile;
};

#endif // BACKEND_H
