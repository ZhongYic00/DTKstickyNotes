#include "transparentwidget.h"

TransparentWidget::TransparentWidget(QWidget *p) : QWidget(p) {
}
void TransparentWidget::updateMask() {
	setMask(QRegion(childrenRegion()));
}
void TransparentWidget::resizeEvent(QResizeEvent *e) {
	updateMask();
	return QWidget::resizeEvent(e);
}
bool TransparentWidget::event(QEvent *e) {
	if (e->type() == QEvent::WindowActivate)
		updateMask();
	return QWidget::event(e);
}
