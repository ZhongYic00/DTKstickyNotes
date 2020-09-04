#ifndef ZNOTE_H
#define ZNOTE_H
#include <QtCore>

class ZNote
{
public:
    ZNote(QString type="attach");
    ZNote(QDateTime create, QDateTime upd, QString html="", QString overview="", QString type="attach");
    ZNote(ZNote &other);
    ZNote(const ZNote &other);
    ZNote(const QJsonObject &obj);
    bool operator < (const ZNote &x);
    bool operator <= (const ZNote &x);
    bool operator > (const ZNote &x);
    bool operator < (const QDateTime &x);
    bool operator > (const QDateTime &x);
    QDateTime lastModified();
    QString getHtml() const;
    QString getOverview() const;
    void setHtml(const QString &_html);
    void setOverview(const QString &_overview);
    void commitChange();
    void toggleAttach();
    QJsonObject jsonObject() const;
    void print() const;
    QString getUpdateTime() const;
    QString getCreateTime() const;
    QString getAttach() const;
    QDateTime getUpdateTimeRaw() const;
    static QString humanDateTime(const QDateTime &from);
private:
    QString html;
    QString overview;
    QString displayType;
    QDateTime createTime;
    QDateTime updateTime;
};

Q_DECLARE_METATYPE(ZNote)

#endif // ZNOTE_H
