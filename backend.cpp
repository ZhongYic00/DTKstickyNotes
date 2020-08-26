#include "backend.h"

ZBackend::ZBackend()
{
}
void ZBackend::save(const QList<ZNote> &src)//下一步实现双文件储存，防数据遗失
{
    QFile output(storageFileName());
    output.open(QFile::WriteOnly|QFile::Text);
    QTextStream outputStream(&output);
    outputStream<<exportAsJson(src).toJson(QJsonDocument::Compact);
    output.close();
}
QList<ZNote> ZBackend::getSavedDataList() const
{
    auto objList=QVariant(QJsonDocument::fromJson(readWholeFile(storageFileName()).toUtf8()).array().toVariantList()).toList();
    QList<ZNote> rt;
    for(auto i:objList)
    {
        auto obj=i.toJsonObject();
        rt.push_back(ZNote(obj));
    }
    return rt;
}
inline QString ZBackend::storageFileName() const
{
    return "stickyNotes.json";
}
inline QString ZBackend::readWholeFile(const QString &filename) const
{
    QFile input(filename);
    input.open(QFile::ReadOnly|QFile::Text);
    QTextStream inputStream(&input);
    QString rt=inputStream.readAll();
//    qDebug()<<"import file content:\n"<<rt;
    input.close();
    return rt;
}
QJsonDocument ZBackend::exportAsJson(const QList<ZNote> &src)
{
    QJsonArray array;
    for(auto i:src)
        array.append(QJsonValue(i.jsonObject()));
    return QJsonDocument(array);
}
