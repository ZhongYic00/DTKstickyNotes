#ifndef ZNOTE_H
#define ZNOTE_H
#include <QTextDocumentFragment>
#include <QtCore>

#define AbstractMaxLength 200
class ZNote {
public:
    enum Attachments {
        Attach,
        Detach
    };
    ZNote(const bool isAttached = true);
    ZNote(const QDateTime& key);
    ZNote(const QDateTime& create, const QDateTime& upd, const QString& html = "", const QString& abstract = "", const bool isAttached = true);
    //ZNote(ZNote& other);
    ZNote(const ZNote& other);
    ZNote(const QJsonObject& obj);

    inline bool operator<(const ZNote& x) const;
    inline bool operator<=(const ZNote& x) const;
    inline bool operator>(const ZNote& x) const;
    /*inline bool operator<(const QDateTime& x);
    inline bool operator>(const QDateTime& x);*/

    inline QDateTime lastModified() const;
    inline QString getHtml() const;
    inline QString getAbstract() const;
    inline bool isAttached() const;
    inline QString getUpdateTime() const;
    inline QString getCreateTime() const;
    inline QDateTime getUpdateTimeRaw() const;
    inline void setContent(const QTextDocumentFragment& doc);
    inline void setHtml(const QString& _html);
    inline void setAbstract(const QString& _abstract);
    inline void commitChange();
    inline void toggleAttach();

    QJsonObject jsonObject() const;

    static QString humanDateTime(const QDateTime& from);

    friend QDebug operator<<(QDebug o, const ZNote& z);

private:
    QDateTime createTime;
    QDateTime updateTime;
    QString html;
    QString abstract;
    bool attachment;
};

bool ZNote::operator<(const ZNote& x) const { return updateTime > x.updateTime; }
bool ZNote::operator<=(const ZNote& x) const { return updateTime >= x.updateTime; }
bool ZNote::operator>(const ZNote& x) const { return updateTime < x.updateTime; }
/*bool ZNote::operator<(const QDateTime& x) { return updateTime > x; }
bool ZNote::operator>(const QDateTime& x) { return updateTime < x; }*/

QDateTime ZNote::lastModified() const { return updateTime; }
QString ZNote::getHtml() const { return html; }
QString ZNote::getAbstract() const { return abstract; }
bool ZNote::isAttached() const { return attachment; }
QString ZNote::getUpdateTime() const { return humanDateTime(updateTime); }
QString ZNote::getCreateTime() const { return humanDateTime(createTime); }
QDateTime ZNote::getUpdateTimeRaw() const { return updateTime; }

void ZNote::setContent(const QTextDocumentFragment& doc)
{
    html = doc.toHtml();
    abstract = doc.toPlainText().left(AbstractMaxLength);
}
void ZNote::setHtml(const QString& _html) { html = _html; }
void ZNote::setAbstract(const QString& _abstract) { abstract = _abstract.left(AbstractMaxLength); }
void ZNote::commitChange() { updateTime = QDateTime::currentDateTime(); }
void ZNote::toggleAttach() { attachment = !attachment; }

Q_DECLARE_METATYPE(ZNote) //to enable storage in QVariant

#endif // ZNOTE_H
