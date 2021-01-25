#ifndef STICKYWIDGET_H
#define STICKYWIDGET_H

#include "roundedwidgets.h"
#include "transparentwidget.h"
#include "znote.h"
#include "ztextedit.h"
#include <DBlurEffectWidget>
#include <DIconButton>
#include <QCursor>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE
class StickyWidget : public DBlurEffectWidget {
    Q_OBJECT
public:
    StickyWidget(QWidget* parent = nullptr);
    inline void setNote(const ZNote& note);
    inline InnerIndex noteIndex() const;
    inline ZTextEdit* textEditor() const;
    //    void commitModifications();
signals:
    void attach();
    void textChanged(const QString& text = "");
    //    void indexChanged(const InnerIndex& ori, const InnerIndex& idx);

protected:
    //    void focusOutEvent(QFocusEvent *e);
private:
    ZTextEdit* editor;
    void initWidgets();
    QWidget* initToolBar(QWidget* parent, ZTextEdit* textEditor);
};
InnerIndex StickyWidget::noteIndex() const { return editor->index(); }
ZTextEdit* StickyWidget::textEditor() const { return editor; }
void StickyWidget::setNote(const ZNote& note) { editor->setNote(note); }
#endif // STICKYWIDGET_H
