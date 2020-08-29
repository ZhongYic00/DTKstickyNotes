#ifndef ROUNDEDWIDGET_H
#define ROUNDEDWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE
class RoundedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoundedWidget(QWidget *parent = nullptr, int r=0);
    void setRadius(int r);
    void setMargin(int d);
    void setShadow(int d);
signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
private:
    int radius;
    int margin;
    QPainter *painter;
    void resetPainter();
};

#endif // ROUNDEDWIDGET_H
