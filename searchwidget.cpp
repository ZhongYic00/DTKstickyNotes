#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent), active(false), freeze(false) {
	input = new DSearchEdit(this);
	result = new SearchResult(this);
	auto layout = new QVBoxLayout(this);
	layout->addWidget(input);
	layout->addWidget(result);
	layout->setMargin(0);
	result->setVisible(false);
	setLayout(layout);
	setAttribute(Qt::WA_TranslucentBackground);
	setFocusPolicy(Qt::ClickFocus);

	connect(input, &DSearchEdit::textEdited, this, &SearchWidget::changeSearchResult);
	connect(input, &DSearchEdit::focusChanged, this, &SearchWidget::reset);
	connect(result, &SearchResult::widgetMoving, this, &SearchWidget::keepPopupPosition);
	connect(result, &SearchResult::changeCurrent, [this](const QModelIndex &cur) {
		emit changeCurrent(cur);
		reset(false);
		this->previousInFocusChain()->setFocus();
	});
}
void SearchWidget::changeSearchResult(const QString &text) {
	if (!active) {
		result->setFixedWidth(input->width());
		result->move(mapToGlobal(QPoint(0, input->height())));
		result->setVisible(true);
		active = true;
	}
	if (text != "\255")
		result->filter(text);
}
void SearchWidget::reset(bool b) {
	//    qDebug()<<"call reset"<<b<<active;
	if ((active || freeze) && b)
		return;
	else if (b)
		return changeSearchResult("\255");
	result->setVisible(false);
	active = false;
	freeze = true;
	QTimer::singleShot(100, this, SLOT(melt()));
}
void SearchWidget::melt() {
	freeze = false;
}
void SearchWidget::moveEvent(QMoveEvent *event) {
	keepPopupPosition();
	//    QWidget::moveEvent(event);
}
void SearchWidget::resizeEvent(QResizeEvent *event) {
	if (active) {
		result->setFixedWidth(input->width());
	}
	//    QWidget::resizeEvent(event);
}
void SearchWidget::keepPopupPosition() {
	if (!active)
		return;
	result->blockSignals(true);
	result->move(mapToGlobal(QPoint(0, input->height())));
	result->blockSignals(false);
}
void SearchWidget::setSourceModel(QAbstractItemModel *m) {
	result->setSrcModel(m);
}
