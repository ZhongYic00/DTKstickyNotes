#include "znote.h"

ZNote::ZNote(const bool isAttached)
    : attachment(isAttached)
{
    updateTime = createTime = QDateTime::currentDateTime();
}
ZNote::ZNote(const QDateTime& key)
    : updateTime(key)
{
}
ZNote::ZNote(const QDateTime& create, const QDateTime& upd, const QString& html, const QString& abstract, const bool isAttached)
    : createTime(create)
    , updateTime(upd)
    , html(html)
    , abstract(abstract)
    , attachment(isAttached)
{
}
ZNote::ZNote(const ZNote& other)
    : createTime(other.createTime)
    , updateTime(other.updateTime)
    , html(other.html)
    , abstract(other.abstract)
    , attachment(other.attachment)
{
}
ZNote::ZNote(const QJsonObject& obj)
    : attachment(true)
{
    //    qDebug()<<obj;
    //if (obj.contains("createTime") && obj.contains("updateTime") && obj.contains("html") && obj.contains("abstract")) {
    createTime = QDateTime::fromString(obj["createTime"].toString(), Qt::ISODate);
    updateTime = QDateTime::fromString(obj["updateTime"].toString(), Qt::ISODateWithMs);
    html = obj["html"].toString();
    abstract = obj["abstract"].toString();
    //}
}

QJsonObject ZNote::jsonObject() const
{
    QJsonObject obj;
    obj["createTime"] = createTime.toString(Qt::ISODate);
    obj["updateTime"] = updateTime.toString(Qt::ISODateWithMs);
    obj["abstract"] = abstract;
    obj["html"] = html;
    return obj;
}
QString ZNote::humanDateTime(const QDateTime& from)
{
    auto cur = QDateTime::currentDateTime();
    if (from.daysTo(cur) == 0) {
        if (from.secsTo(cur) <= 60)
            return QObject::tr("刚刚");
        else if (from.secsTo(cur) <= 3600)
            return QObject::tr("%n minute(s) ago", "", int(from.secsTo(cur) / 60));
        else if (from.secsTo(cur) <= 3600 * 3)
            return QObject::tr("%n hour(s) ago", "", int(from.secsTo(cur) / 3600));
        else
            return from.toString("hh:mm:ss");
    } else if (from.daysTo(cur) <= 3)
        return QObject::tr("%n day(s) ago", "", int(from.daysTo(cur)));
    else
        return from.toString("yyyy-MM-dd hh:mm");
}
QDebug operator<<(QDebug o, const ZNote& z)
{
    o << z.jsonObject() << endl;
    return o;
}
