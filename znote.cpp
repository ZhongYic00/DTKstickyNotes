#include "znote.h"

ZNote::ZNote(const QString type)
    : displayType(type)
{
    updateTime = createTime = QDateTime::currentDateTime();
}
ZNote::ZNote(const QDateTime& key)
    : updateTime(key)
{
}
ZNote::ZNote(const QDateTime& create, const QDateTime& upd, const QString& html, const QString& overview, const QString& type)
    : createTime(create)
    , updateTime(upd)
    , html(html)
    , overview(overview)
    , displayType(type)
{
}
ZNote::ZNote(ZNote& other)
    : createTime(other.createTime)
    , updateTime(other.updateTime)
    , html(other.html)
    , overview(other.overview)
    , displayType(other.displayType)
{
}
ZNote::ZNote(const ZNote& other)
    : createTime(other.createTime)
    , updateTime(other.updateTime)
    , html(other.html)
    , overview(other.overview)
    , displayType(other.displayType)
{
}
ZNote::ZNote(const QJsonObject& obj)
    : displayType("attach")
{
    //    qDebug()<<obj;
    if (obj.contains("createTime") & obj.contains("updateTime") & obj.contains("html") & obj.contains("overview")) {
        createTime = QDateTime::fromString(obj["createTime"].toString());
        updateTime = QDateTime::fromString(obj["updateTime"].toString());
        html = obj["html"].toString();
        overview = obj["overview"].toString();
    }
}

QJsonObject ZNote::jsonObject() const
{
    QJsonObject obj;
    obj["createTime"] = createTime.toString();
    obj["updateTime"] = updateTime.toString();
    obj["overview"] = overview;
    obj["html"] = html;
    return obj;
}
void ZNote::print() const
{
    //    qDebug()<<overview<<displayType;
    qDebug() << jsonObject();
}
QString ZNote::humanDateTime(const QDateTime& from)
{
    auto cur = QDateTime::currentDateTime();
    if (from.daysTo(cur) == 0) {
        if (from.secsTo(cur) <= 10)
            return QObject::tr("刚刚");
        else
            return from.toString("hh:mm:ss");
    } else if (from.daysTo(cur) <= 3)
        return QString::number(from.daysTo(cur)) + QObject::tr("天前");
    else
        return from.toString("yyyy MM dd hh:mm");
}
