#include "daemon.h"
Daemon* Daemon::daemon = nullptr;
Daemon::Daemon(QObject* parent)
    : QObject(parent)
{
    assert(daemon == nullptr);

    if (daemon == nullptr)
        daemon = this;

    back = new ZBackend;
    model = new ZListModel(this);

    addItems(back->getSavedDataList());
}
Daemon::~Daemon()
{
    assert(daemon == this);
    if (daemon == this)
        daemon = nullptr;
}
void Daemon::addItem(const ZNote& item)
{
    qDebug() << "Daemon::addItem";
    model->appendRow(item);
    qDebug() << "items:" << model->rowCount();
}
void Daemon::addItems(const QList<ZNote>& items)
{
    for (auto i : items)
        addItem(i);
}
void Daemon::removeItem(const ZNote& item)
{
    model->removeRow(item);
}
void Daemon::removeItems(const QList<ZNote>& items)
{
    for (auto i : items)
        removeItem(i);
}
QList<ZNote> Daemon::getDataList() const
{
    return model->exportAll();
}
void Daemon::setOverview(const InnerIndex& idx, const QString& overview)
{
    model->setData(idx, QVariant::fromValue(overview), ZListModel::Overview);
}
void Daemon::setHtml(const InnerIndex& idx, const QString& html)
{
    model->setData(idx, QVariant::fromValue(html), ZListModel::Html);
}
InnerIndex Daemon::commitChange(const InnerIndex& idx, bool toggleAttach) //优化逻辑，若通过save主动commit，后续setIndex时可能再次触发commit
{
    if (toggleAttach)
        model->setData(idx, QVariant(), ZListModel::Attachment);
    return model->setData(idx, QVariant(), ZListModel::UpdateTime);
}
void Daemon::commitChange(const QModelIndex& index, bool toggleAttach) //优化逻辑，若通过save主动commit，后续setIndex时可能再次触发commit
{
    if (toggleAttach)
        model->setData(index, QVariant(), ZListModel::Attachment);
    model->setData(index, QVariant(), ZListModel::UpdateTime);
}
ZListModel* Daemon::getModel()
{
    return model;
}
void Daemon::save()
{
    back->saveMainFile(model->exportAll());
    back->saveMediaFile();
}
QObject* Daemon::systemTray()
{
    return parent();
}
bool Daemon::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::ActivationChange) {
        emit activationChanged(obj);
        if (auto i = qobject_cast<QWidget*>(obj)) {
            i->isActiveWindow() ? emit gainActivation(i) : emit lostActivation(i);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, e);
    }
}
void Daemon::detach(InnerIndex idx)
{
    idx = commitChange(idx, true);
    //    emit itemDetached(model->indexOf(idx));
    emit itemDetached(idx);
}
Daemon* Daemon::instance()
{
    assert(daemon != nullptr);
    return daemon;
}
QString Daemon::calcImageHash(const QUrl& fileUrl)
{
    return back->insertImage(fileUrl);
}
QString Daemon::calcImageHash(const QByteArray& data)
{
    return back->insertImage(data);
}
QByteArray Daemon::fetchImageData(const QString& hash)
{
    return back->queryImage(hash);
}
QByteArray Daemon::fetchRemoteResource(const QString& url)
{
    return back->download(url);
}
