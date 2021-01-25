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
    tray = new Systemtray(this);

    tray->show();

    addItems(back->getSavedDataList());
}
Daemon::~Daemon()
{
    assert(daemon == this);
    if (daemon == this)
        daemon = nullptr;
}

QList<ZNote> Daemon::exportNotes() const
{
    return model->exportAll();
}
void Daemon::setAbstract(const InnerIndex& idx, const QString& Abstract)
{
    model->setData(idx, QVariant::fromValue(Abstract), ZListModel::Abstract);
}
void Daemon::setHtml(const InnerIndex& idx, const QString& html)
{
    model->setData(idx, QVariant::fromValue(html), ZListModel::Html);
}
InnerIndex Daemon::commitChange(const InnerIndex& idx) //优化逻辑，若通过save主动commit，后续setIndex时可能再次触发commit
{
    qDebug() << "commitChange(" << idx << ")";
    return model->setData(idx, QVariant(), ZListModel::UpdateTime);
}
void Daemon::commitChange(const QModelIndex& index) //优化逻辑，若通过save主动commit，后续setIndex时可能再次触发commit
{
    model->setData(index, QVariant(), ZListModel::UpdateTime);
}
void Daemon::save()
{
    back->saveMainFile(model->exportAll());
    back->saveMediaFile();
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
