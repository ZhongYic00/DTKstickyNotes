#include "znote.h"

ZNote::ZNote()
{
    updateTime=createTime=QDateTime::currentDateTime();
}
ZNote::ZNote(QDateTime create,QDateTime upd,QString html,QString overview):createTime(create),updateTime(upd),html(html),overview(overview){}
ZNote::ZNote(ZNote &other):createTime(other.createTime),updateTime(other.updateTime),html(other.html),overview(other.overview){}
ZNote::ZNote(const ZNote &other):createTime(other.createTime),updateTime(other.updateTime),html(other.html),overview(other.overview){}
ZNote::ZNote(const QJsonObject &obj)
{
//    qDebug()<<obj;
    if(obj.contains("createTime")&obj.contains("updateTime")&obj.contains("html")&obj.contains("overview"))
        createTime=QDateTime::fromString(obj["createTime"].toString()),
                updateTime=QDateTime::fromString(obj["updateTime"].toString()),
                html=obj["html"].toString(),
                overview=obj["overview"].toString();
}

bool ZNote::operator<(const ZNote &x)
{
    return updateTime>x.updateTime;
}
bool ZNote::operator<=(const ZNote &x)
{
    return updateTime>=x.updateTime;
}
QDateTime ZNote::lastModified()
{
    return updateTime;
}
QString ZNote::getHtml() const
{
    return html;
}
QString ZNote::getOverview() const
{
    return overview;
}
void ZNote::setHtml(const QString &_html)
{
    html=_html;
    updateTime=QDateTime::currentDateTime();
}
void ZNote::setOverview(const QString &_overview)
{
    overview=_overview;
}
QJsonObject ZNote::jsonObject() const
{
    QJsonObject obj;
    obj["createTime"]=createTime.toString();
    obj["updateTime"]=updateTime.toString();
    obj["overview"]=overview;
    obj["html"]=html;
    return obj;
}
void ZNote::print() const
{
    qDebug()<<jsonObject();
}
QString ZNote::humanDateTime(const QDateTime &from)
{
    auto cur=QDateTime::currentDateTime();
    if(from.daysTo(cur)==0)
    {
        if(from.secsTo(cur)<=10)
            return QObject::tr("刚刚");
        else return from.toString("hh:mm:ss");
    }
    else if(from.daysTo(cur)<=3)return QString::number(from.daysTo(cur))+QObject::tr("天前");
    else return from.toString("yyyy MM dd hh:mm");
}
QString ZNote::getUpdateTime() const
{
    return humanDateTime(updateTime);
}
QString ZNote::getCreateTime() const
{
    return humanDateTime(createTime);
}
