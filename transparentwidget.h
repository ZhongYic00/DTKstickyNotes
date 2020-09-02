#ifndef TRANSPARENTWIDGET_H
#define TRANSPARENTWIDGET_H

#include <QWidget>
#include <QEvent>

class TransparentWidget:public QWidget
{
    Q_OBJECT
public:
    TransparentWidget(QWidget *parent=nullptr);
private:
    void resizeEvent(QResizeEvent*);
    void updateMask();
    bool event(QEvent*);
};

#endif // TRANSPARENTWIDGET_H
