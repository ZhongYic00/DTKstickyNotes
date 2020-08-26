#ifndef ZNOTE_H
#define ZNOTE_H
#include <QtCore>

class ZNote
{
public:
    ZNote();
    ZNote(QDateTime create,QDateTime upd,QString html="",QString overview="");
    ZNote(ZNote &other);
    ZNote(const ZNote &other);
    ZNote(const QJsonObject &obj);
    bool operator < (const ZNote &x);
    bool operator <= (const ZNote &x);
    QDateTime lastModified();
    QString getHtml()const;
    QString getOverview()const;
    void setHtml(const QString &_html);
    void setOverview(const QString &_overview);
    void commitChange();
    QJsonObject jsonObject()const;
    void print() const;
    QString getUpdateTime() const;
    QString getCreateTime() const;
    static QString humanDateTime(const QDateTime &from);
private:
    QString html;
    QString overview;
    QDateTime createTime;
    QDateTime updateTime;
};

Q_DECLARE_METATYPE(ZNote)

#endif // ZNOTE_H
