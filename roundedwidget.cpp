#include "roundedwidget.h"

DGUI_USE_NAMESPACE
RoundedWidget::RoundedWidget(QWidget *parent, int r) : QWidget(parent), radius(r), margin(0), painter(nullptr)
{
    setPalette(DGuiApplicationHelper::instance()->applicationPalette());
    connect(DGuiApplicationHelper::instance(),&DGuiApplicationHelper::themeTypeChanged,[this](){
        setPalette(DGuiApplicationHelper::instance()->applicationPalette());
        repaint();
    });
}
void RoundedWidget::paintEvent(QPaintEvent *event)
{
    if(!painter)
    {
        painter=new QPainter(this);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
    painter->begin(this);
    QPainterPath path;
    auto rect=event->rect();
    if(margin)
        rect.adjust(margin,margin,-margin,-margin);
    path.addRoundedRect(QRectF(rect),radius,radius);
    painter->fillPath(path,palette().window());
    painter->end();
}
void RoundedWidget::setRadius(int r)
{
    radius=r;
}
void RoundedWidget::setMargin(int d)    //set self margins
{
    margin=d;
    setContentsMargins(margin,margin,margin,margin);
}
void RoundedWidget::setShadow(int d)
{
    if(!d)return ;
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0,0);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(d);
    setGraphicsEffect(effect);
    setMargin(d);
}
