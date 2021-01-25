#ifndef EDITOR_H
#define EDITOR_H

#include "transparentwidget.h"
#include "ztextedit.h"
#include <DInputDialog>
#include <QtWidgets>
#include <dboxwidget.h>

DWIDGET_USE_NAMESPACE
typedef std::pair<QString, QString> pss;
class Editor : public QWidget {
    Q_OBJECT
public:
    Editor(QWidget* parent = nullptr);
    inline void setNote(const ZNote& note);
    void reset();
signals:
    void indexUpdated(const InnerIndex& idx);

protected:
    //    void keyPressEvent(QKeyEvent*);
    //    void keyReleaseEvent(QKeyEvent*);
    //    void mousePressEvent(QMouseEvent*);
private:
    QWidget* initToolBar();
    QWidget* initToolButtons(QWidget*);

    ZTextEdit* editor;
};
void Editor::setNote(const ZNote& note)
{
    editor->setNote(note);
    setEnabled(true);
    editor->setFocus();
}
#endif // EDITOR_H
