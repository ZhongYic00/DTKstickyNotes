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

DWIDGET_USE_NAMESPACE
class StickyWidget : public DBlurEffectWidget
{
public:
    StickyWidget(QWidget *parent=nullptr);
private:
    void initWidgets();
    QWidget* initToolBar(QWidget *parent,ZTextEdit *textEditor);
};

#endif // STICKYWIDGET_H
