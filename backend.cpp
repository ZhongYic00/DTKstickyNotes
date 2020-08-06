#include "backend.h"

ZBackend::ZBackend()
{
    QFile input("stickyNotes.json");
    input.open(QFile::ReadOnly|QFile::Text);
    QTextStream jsonText(&input);
//    QString jsonTextAll=jsonText.readAll();
    auto tmpStrList=QVariant(QJsonDocument::fromJson(jsonText.readAll().toUtf8()).array().toVariantList()).toList();
//    qDebug()<<tmpStrList;
    for(auto i:tmpStrList)
    {
        auto j=i.toJsonObject();
        data[QDateTime::fromString(j["updateTime"].toString())]=
                new ZNote(QDateTime::fromString(j["createTime"].toString()),QDateTime::fromString(j["updateTime"].toString()),j["html"].toString(),j["overview"].toString());
    }
    input.close();
    tmpFile.setFileName("~stickyNotes.json");
    tmpFile.open(QFile::WriteOnly);
}
ZNote* ZBackend::addNote()
{
    auto cur=QDateTime::currentDateTime();
    data[cur]=new ZNote(cur,cur);
    save();
    return data[cur];
}
void ZBackend::removeNote(ZNote *note)
{
    if(note)
    {
        data.erase(note->lastModified());
        delete note;
    }
}
void ZBackend::updateNote(QDateTime lastModified, QString rich)
{
    if(data[lastModified])
    {
        auto mdf=data[lastModified];
        mdf->update(rich,mdf->getOverview());
        data.erase(lastModified);
        data[mdf->lastModified()]=mdf;
    }
    save();
}
void ZBackend::updateOverview(QDateTime lastModified, QString plain)
{
    if(data[lastModified])data[lastModified]->update(plain);
}
void ZBackend::save()
{
    QFile output("stickyNotes.json");
    output.open(QFile::WriteOnly|QFile::Text);
    QTextStream outputStream(&output);
    QJsonArray array;
    for(auto i:data)
    {
        array.append(QJsonValue(i.second->printObject()));
    }
    QJsonDocument doc;
    doc.setArray(array);
    outputStream<<doc.toJson(QJsonDocument::Compact);
    output.close();
}
std::vector<std::pair<QDateTime,ZNote*> > ZBackend::getDataList() const
{
    return std::vector<std::pair<QDateTime,ZNote*> >(data.begin(),data.end());
}
