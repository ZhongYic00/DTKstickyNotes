#include "roundedwidgets.h"
#include <QDebug>

DGUI_USE_NAMESPACE
RoundedWidget::RoundedWidget(QWidget *parent, int r, int t, QPoint o)
: QWidget(parent), radius(r), margin(0), type(t), offset(o), painter(nullptr) {
	setPalette(DGuiApplicationHelper::instance()->applicationPalette());
	connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, [this]() {
		setPalette(DGuiApplicationHelper::instance()->applicationPalette());
		repaint();
	});
	if (!offset.isNull())
		connect(this, &RoundedWidget::widgetMoving, [=](QPoint pos) {
			this->blockSignals(true);
			this->move(pos + this->offset);
			this->blockSignals(false);
		});
}
void RoundedWidget::paintEvent(QPaintEvent *event) {
	if (!painter) {
		painter = new QPainter(this);
		painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	}
	painter->begin(this);
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QPainterPath path;
	auto rect = event->rect();
	if (margin)
		rect.adjust(margin, margin, -margin, -margin);
	path.addRoundedRect(QRectF(rect), radius, radius);
	if (type) {
		auto tmpRect = rect;
		switch (type) {
			case TopTwo:
				tmpRect.adjust(0, rect.height() - radius, 0, 0);
				break;
			case BottomTwo:
				tmpRect.adjust(0, 0, 0, -(rect.height() - radius));
				break;
			case LeftTwo:
				tmpRect.adjust(0, 0, -(rect.width() - radius), 0);
				break;
			case RightTwo:
				tmpRect.adjust(rect.width() - radius, 0, 0, 0);
				break;
		}
		path.addRect(QRectF(tmpRect));
	}
	path.setFillRule(Qt::WindingFill);
	painter->fillPath(path, palette().window());
	painter->end();
}
void RoundedWidget::moveEvent(QMoveEvent *event) {
	emit widgetMoving(event->pos());
	return QWidget::moveEvent(event);
}
void RoundedWidget::setRadius(int r) {
	radius = r;
}
void RoundedWidget::setMargin(int d) // set self margins
{
	margin = d;
	setContentsMargins(margin, margin, margin, margin);
}
void RoundedWidget::setShadow(int d) {
	if (!d)
		return;
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
	effect->setOffset(0, 0);
	effect->setColor(QColor(0, 0, 0, 50));
	effect->setBlurRadius(d);
	setGraphicsEffect(effect);
	setMargin(d);
}
CircleButton::CircleButton(QIcon icon, QWidget *parent) : QPushButton(icon, "", parent), painter(nullptr) {
	setPalette(DGuiApplicationHelper::instance()->applicationPalette());
	connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, [this]() {
		setPalette(DGuiApplicationHelper::instance()->applicationPalette());
		repaint();
	});
}
void CircleButton::paintEvent(QPaintEvent *event) {
	if (!painter) {
		painter = new QPainter(this);
		painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	}
	//	qDebug() << "renderHints:" << (painter->renderHints() == (QPainter::Antialiasing | QPainter::SmoothPixmapTransform));
	//	painter->save();
	painter->begin(this);
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QPainterPath path;
	path.addEllipse(event->rect());
	painter->fillPath(path, isChecked() ? palette().highlight() : palette().button());
	auto iconRect = event->rect();
	int d = int(iconRect.width() * 0.3);
	iconRect.adjust(d, d, -d, -d);
	painter->drawPixmap(iconRect, this->icon().pixmap(iconRect.width(), iconRect.height()));
	painter->end();
}
