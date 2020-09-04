#ifndef STICKYWIDGET_H
#define STICKYWIDGET_H
#include <DBlurEffectWidget>
#include <DIconButton>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCursor>
#include "transparentwidget.h"
#include "ztextedit.h"
#include "roundedwidgets.h"
#include "znote.h"

DWIDGET_USE_NAMESPACE
class StickyWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:
    StickyWidget(QObject *parent=nullptr);
    void setNote(ZNote &d);
    ZNote getNote();
signals:
    void attach();
protected:
//    void focusOutEvent(QFocusEvent *e);
private:
    ZNote note;
    ZTextEdit *editor;
    void initWidgets();
    QWidget* initToolBar(QWidget *parent,ZTextEdit *textEditor);
};

#endif // STICKYWIDGET_H
