#ifndef ZNOTE_H
#define ZNOTE_H
#include <QtCore>
class ZNote {
public:
    enum Attachments {
        Attach,
        Detach
    };
    ZNote(const bool isAttached = true);
    ZNote(const QDateTime& key);
    ZNote(const QDateTime& create, const QDateTime& upd, const QString& html = "", const QString& overview = "", const bool isAttached = true);
    //ZNote(ZNote& other);
    ZNote(const ZNote& other);
    ZNote(const QJsonObject& obj);

    inline bool operator<(const ZNote& x);
    inline bool operator<=(const ZNote& x);
    inline bool operator>(const ZNote& x);
    /*inline bool operator<(const QDateTime& x);
    inline bool operator>(const QDateTime& x);*/

    inline QDateTime lastModified() const;
    inline QString getHtml() const;
    inline QString getOverview() const;
    inline bool isAttached() const;
    inline QString getUpdateTime() const;
    inline QString getCreateTime() const;
    inline QDateTime getUpdateTimeRaw() const;
    inline void setHtml(const QString& _html);
    inline void setOverview(const QString& _overview);
    inline void commitChange();
    inline void toggleAttach();

    static QString humanDateTime(const QDateTime& from);

    QJsonObject jsonObject() const;
    void print() const;

private:
    QDateTime createTime;
    QDateTime updateTime;
    QString html;
    QString overview;
    bool attachment;
};

bool ZNote::operator<(const ZNote& x) { return updateTime > x.updateTime; }
bool ZNote::operator<=(const ZNote& x) { return updateTime >= x.updateTime; }
bool ZNote::operator>(const ZNote& x) { return updateTime < x.updateTime; }
/*bool ZNote::operator<(const QDateTime& x) { return updateTime > x; }
bool ZNote::operator>(const QDateTime& x) { return updateTime < x; }*/

QDateTime ZNote::lastModified() const { return updateTime; }
QString ZNote::getHtml() const { return html; }
QString ZNote::getOverview() const { return overview; }
bool ZNote::isAttached() const { return attachment; }
QString ZNote::getUpdateTime() const { return humanDateTime(updateTime); }
QString ZNote::getCreateTime() const { return humanDateTime(createTime); }
QDateTime ZNote::getUpdateTimeRaw() const { return updateTime; }

void ZNote::setHtml(const QString& _html) { html = _html; }
void ZNote::setOverview(const QString& _overview) { overview = _overview; }
void ZNote::commitChange() { updateTime = QDateTime::currentDateTime(); }
void ZNote::toggleAttach() { attachment = !attachment; }

Q_DECLARE_METATYPE(ZNote) //to enable storage in QVariant

#endif // ZNOTE_H
