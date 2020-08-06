#ifndef ZNOTE_H
#define ZNOTE_H
#include <QtCore>

class ZNote
{
public:
    ZNote();
    ZNote(QDateTime create,QDateTime upd,QString html="",QString overview="");
    bool operator < (const ZNote &x);
    void update(QString rich,QString plain);
    void update(QString plain);
    QDateTime lastModified();
    QString getHtml()const;
    QString getOverview()const;
    QJsonObject printObject()const;
private:
    QString html;
    QString overview;
    QDateTime createTime;
    QDateTime updateTime;
};

#endif // ZNOTE_H
