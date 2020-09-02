#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets>
#include <dboxwidget.h>
#include <DInputDialog>
#include "ztextedit.h"
#include "transparentwidget.h"

DWIDGET_USE_NAMESPACE
typedef std::pair<QString,QString> pss;
class Editor:public QWidget
{
    Q_OBJECT
public:
    Editor(QWidget *parent=nullptr);
    void display(const QString &html);
    void reset();
    QString getContentRich() const;
signals:
    void contentChanged(const pss);
    void contentSaved(const QString);
protected:
//    void keyPressEvent(QKeyEvent*);
//    void keyReleaseEvent(QKeyEvent*);
//    void mousePressEvent(QMouseEvent*);
private:
    ZTextEdit *textEditor;
    QWidget *toolBar;
    QWidget* initToolBar();
    QWidget* initToolButtons(QWidget*);
};

#endif // EDITOR_H
