#include "znote.h"

ZNote::ZNote()
{
    updateTime=createTime=QDateTime::currentDateTime();
}
ZNote::ZNote(QDateTime create,QDateTime upd,QString html,QString overview):createTime(create),updateTime(upd),html(html),overview(overview){}

void ZNote::update(QString rich, QString plain)
{
    html=rich;
    overview=plain.left(100);
    updateTime=QDateTime::currentDateTime();
}
void ZNote::update(QString plain)
{
    overview=plain.left(100);
}
bool ZNote::operator<(const ZNote &x)
{
    return updateTime<x.updateTime;
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
QJsonObject ZNote::printObject() const
{
    QJsonObject obj;
    obj["createTime"]=createTime.toString();
    obj["updateTime"]=updateTime.toString();
    obj["overview"]=overview;
    obj["html"]=html;
    return obj;
}
