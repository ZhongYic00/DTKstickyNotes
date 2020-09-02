#ifndef ROUNDEDWIDGET_H
#define ROUNDEDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE
class RoundedWidget : public QWidget
{
    Q_OBJECT
public:
    enum CornerType{All, TopTwo, BottomTwo, LeftTwo, RightTwo};
    explicit RoundedWidget(QWidget *parent = nullptr, int r=0, int t=0, QPoint offset=QPoint());
    void setRadius(int r);
    void setMargin(int d);
    void setShadow(int d);
signals:
    void widgetMoving(QPoint pos);
public slots:
protected:
    void paintEvent(QPaintEvent *event);
    void moveEvent(QMoveEvent *event);
private:
    int radius;
    int margin;
    const int type;
    const QPoint offset;
    QPainter *painter;
    void resetPainter();
};
class CircleButton : public QPushButton
{
public:
    CircleButton(QIcon icon,QWidget *parent=nullptr);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QPainter *painter;
};

#endif // ROUNDEDWIDGET_H
