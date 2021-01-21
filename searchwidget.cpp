#include "searchwidget.h"
#include <QApplication>

SearchWidget::SearchWidget(QWidget* parent)
    : QWidget(parent)
{
    input = new DSearchEdit(this);
    result = new SearchResult(this, input);

    //result->installEventFilter(this);
    //    installEventFilter(this);
    //input->installEventFilter(this);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(input, Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(input, &DSearchEdit::textEdited, result, &SearchResult::filter);
    connect(input, &DSearchEdit::focusChanged, [=](bool b) mutable {if(!b)result->filter(""); });
    //connect(input, &DSearchEdit::focusChanged, this, &SearchWidget::reset);
    //connect(result, &SearchResult::widgetMoving, this, &SearchWidget::keepPopupPosition);
}
void SearchWidget::resizeEvent(QResizeEvent* e)
{
    auto pos = mapToGlobal(input->rect().bottomLeft());
    result->move(pos.x(), pos.y() + 5);
    result->setFixedWidth(width());
    QWidget::resizeEvent(e);
    //result->show();
}
void SearchWidget::setModel(QAbstractItemModel* m)
{
    result->setModel(m);
}
bool SearchWidget::eventFilter(QObject* o, QEvent* e)
{
    //    qDebug() << e;
    //    if (o == input && e->type() == QEvent::FocusAboutToChange) {
    //        input->setFocus();
    //        return true;
    //    }
    if (o != result)
        return QObject::eventFilter(o, e);
    switch (e->type()) {
    case QEvent::KeyPress: {
        eatFocusOut = false;
        static_cast<QObject*>(input)->event(e);
        eatFocusOut = true;
        if (e->isAccepted() || !result->isVisible()) {
            //input lost focus, hide result
            result->hide();
            if (e->isAccepted())
                return true;
        }
        return false;
    }
    case QEvent::KeyRelease: {
        static_cast<QObject*>(input)->event(e);
        break;
    }
    case QEvent::MouseButtonPress: {
        qDebug() << e;
        qDebug() << result->rect();
        auto source = qobject_cast<QWidget*>(o);
        if (source) {
            QWidget* target = QApplication::widgetAt(source->mapToGlobal(static_cast<QMouseEvent*>(e)->pos()));
            if ((target && input->isAncestorOf(target)) || input == target) {
                static_cast<QObject*>(target)->event(e);
                return true;
            }
        }
        if (!result->underMouse()) {
            result->setVisible(false);
            return true;
        }
        return false;
    }
    case QEvent::InputMethod:
    case QEvent::ShortcutOverride:
        QCoreApplication::sendEvent(input, e);
        break;
    default:
        return false;
    }

    return false;
}
