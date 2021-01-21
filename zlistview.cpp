#include "zlistview.h"
#include "editor.h"
#include <DGuiApplicationHelper>
#include <DPlatformTheme>
#include <DPushButton>
#include <DThemeManager>
#include <QDebug>
#include <QtWidgets>

DGUI_USE_NAMESPACE
DTK_USE_NAMESPACE
ZListView::ZListView(QWidget* parent)
    : QListView(parent)
{
    setDragEnabled(false);
    setBatchSize(100);
    setLayoutMode(QListView::Batched);
    setFlow(QListView::TopToBottom);
    setWrapping(false);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformItemSizes(true);
    setSpacing(2);
    viewport()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    setItemDelegate(new ItemDelegate(this));

    connect(this, &QListView::clicked, [this](const QModelIndex& cur) { emit curIndexChanged(cur); });
}
QList<QModelIndex> ZListView::selection() const
{
    return selectedIndexes();
}
QList<ZNote> ZListView::selectionNotes() const
{
    auto indexes = selectedIndexes();
    QList<ZNote> rt;
    for (auto i : indexes)
        rt.push_back(i.data(Qt::UserRole).value<ZNote>());
    return rt;
}
void ZListView::clearSelectionExt()
{
    if (!selectionModel()->hasSelection())
        return;
    int front = selectedIndexes().front().row(), back = selectedIndexes().back().row();
    //    qDebug()<<front<<back<<model()->rowCount();
    clearSelection();
    if (model()->rowCount() > back + 1)
        setCurrentIndex(model()->index(back + 1, 0));
    else if (front != 0)
        setCurrentIndex(model()->index(front - 1, 0));
    else
        emit listEmptied();
}
void ZListView::setCurrentIndex(const QModelIndex& cur)
{
    //    if(cur.isValid()&&cur.model()==model())qDebug()<<"call ZListView::setCurrentIndex"<<cur;
    clearSelection();
    selectionModel()->setCurrentIndex(cur, QItemSelectionModel::SelectCurrent);
    emit curIndexChanged(cur);
}
void ZListView::setNoBackground(bool b)
{
    if (!b)
        return;
    viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);
}

ItemDelegate::ItemDelegate(QWidget* parent)
    : QStyledItemDelegate(parent)
{
}
void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //    qDebug() << "paint" << option << index;
    if (!index.isValid())
        return;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect rect(option.rect);
    rect.adjust(5, 5, -5, -5);
    QPainterPath path;
    path.addRoundRect(QRectF(rect), 30);
    painter->setOpacity(1);
    if (option.state.testFlag(QStyle::State_Selected))
        painter->fillPath(path, option.palette.dark());
    else if (option.state.testFlag(QStyle::State_MouseOver))
        painter->fillPath(path, option.palette.midlight());
    else
        painter->fillPath(path, option.palette.base());

    if (option.state.testFlag(QStyle::State_Selected)) {
        QRect borderRect(rect);
        borderRect.adjust(-3, -3, 3, 3);
        QPainterPath borderPath;
        borderPath.addRoundRect(QRectF(borderRect), 33);
        painter->setPen(QPen(option.palette.highlight(), 2));
        painter->drawPath(borderPath);
    }

    auto data = index.data(Qt::UserRole).value<ZNote>();
    rect.adjust(5, 5, -5, -5);
    QRect mainRect(rect), infoRect(rect);
    mainRect.adjust(0, 0, 0, -20);
    infoRect.adjust(0, 40, 0, 0);
    painter->setPen(QPen(option.palette.text(), 1));
    painter->drawText(QRectF(mainRect), QFontMetrics(painter->font()).elidedText(data.getOverview(), Qt::ElideRight, 500));
    auto font = painter->font();
    font.setItalic(true);
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(QPen(Qt::gray, 1));
    painter->drawText(QRectF(infoRect), QString("%1 %2   %3 %4").arg(QObject::tr("上次修改"), data.getUpdateTime(), QObject::tr("创建于"), data.getCreateTime()));

    painter->restore();
}
QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 80);
}
