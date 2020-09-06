#include "ztextedit.h"
#include "mdialog.h"
#include <DGuiApplicationHelper>
#include <DInputDialog>
#include <QDebug>

DGUI_USE_NAMESPACE
ZTextEdit::ZTextEdit(QWidget *parent, bool DStyle)
: DTextEdit(parent), ctrlPressed(false), hackDStyle(!DStyle), marginBottom(0), marginTop(0) {
	auto p = palette();
	p.setBrush(QPalette::Dark, QBrush(QColor(130, 130, 130)));
	setPalette(p);
	setTextInteractionFlags(textInteractionFlags() | Qt::LinksAccessibleByMouse);
	connect(this, &DTextEdit::currentCharFormatChanged, this, &ZTextEdit::updateCharFormat);
	sample = textCursor().charFormat();
}

void ZTextEdit::keyPressEvent(QKeyEvent *e) {

	if (e->key() == Qt::Key_Control)
		ctrlPressed = true;
	DTextEdit::keyPressEvent(e);
}
void ZTextEdit::keyReleaseEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Control)
		ctrlPressed = false;
	DTextEdit::keyPressEvent(e);
}
void ZTextEdit::mouseReleaseEvent(QMouseEvent *e) {
	if (ctrlPressed) {
		auto cursor = textCursor();
		if (cursor.charFormat().isAnchor())
			QDesktopServices::openUrl(QUrl(cursor.charFormat().anchorHref()));
	}
	QWidget::mousePressEvent(e);
}
bool ZTextEdit::event(QEvent *e) {
	if (hackDStyle)
		return QTextEdit::event(e);
	else
		return DTextEdit::event(e);
}

void ZTextEdit::insertFromMimeData(const QMimeData *src) {
	qDebug() << src->formats();
	if (!src->hasHtml() && src->hasImage()) {
		qDebug() << "insert image detected";
		DTextEdit::insertFromMimeData(src);
	} else {
		qDebug() << "insert html detected";
		auto cursor = textCursor();
		cursor.beginEditBlock();
		auto bak = cursor.charFormat();
		auto blkbak = cursor.blockFormat();
		auto fmt = QTextCharFormat(bak);
		fmt.clearProperty(QTextCharFormat::FontWeight);
		fmt.clearProperty(QTextCharFormat::FontItalic);
		fmt.clearProperty(QTextCharFormat::FontUnderline);
		fmt.clearProperty(QTextCharFormat::FontStrikeOut);
		fmt.setProperty(QTextCharFormat::FontPointSize, bak.property(QTextCharFormat::FontPointSize));
		fmt.setForeground(bak.foreground());
		fmt.setBackground(bak.background());
		auto blkfmt = QTextBlockFormat();
		blkfmt.setBackground(bak.background());
		blkfmt.setProperty(QTextBlockFormat::FontPointSize, blkbak.property(QTextBlockFormat::FontPointSize));
		blkfmt.setProperty(QTextBlockFormat::LineHeight, 0);
		blkfmt.setBottomMargin(0);
		blkfmt.setTopMargin(0);
		if (cursor.hasSelection())
			cursor.removeSelectedText();
		int uninsertedPos = cursor.position();
		if (!uninsertedPos)
			cursor.insertText(" ");
		cursor.insertHtml(src->html());
		int insertedPos = cursor.position();
		cursor.setPosition(uninsertedPos, QTextCursor::MoveAnchor);
		cursor.setPosition(insertedPos, QTextCursor::KeepAnchor);
		cursor.mergeCharFormat(fmt);
		cursor.mergeBlockFormat(blkfmt);
		cursor.setPosition(insertedPos, QTextCursor::MoveAnchor);
		setTextBackgroundColor(bak.background().color());
		if (!uninsertedPos) //在空白textEdit中粘贴文本会，然后全选删除，会出现原因不明的光标下格式错误。
		{
			QTextCursor csr(cursor);
			csr.setPosition(0);
			csr.deleteChar();
		}
		setCurrentCharFormat(bak);
		cursor.endEditBlock();
	}
}

inline void ZTextEdit::fEdit(QTextCharFormat &fmt) {
	auto selection = textCursor();
	selection.beginEditBlock();
	if (!selection.hasSelection())
		mergeCurrentCharFormat(fmt);
	else
		selection.mergeCharFormat(fmt);
	selection.endEditBlock();
}
void ZTextEdit::fBold(bool b) {
	auto fmt = QTextCharFormat();
	fmt.setFontWeight(b ? QFont::DemiBold : QFont::Normal);
	fEdit(fmt);
}
void ZTextEdit::fItalic(bool b) {
	auto fmt = QTextCharFormat();
	fmt.setFontItalic(b);
	fEdit(fmt);
}
void ZTextEdit::fUnderline(bool b) {
	auto fmt = QTextCharFormat();
	fmt.setFontUnderline(b);
	fEdit(fmt);
}
void ZTextEdit::fLinethrough(bool b) {
	auto fmt = QTextCharFormat();
	fmt.setFontStrikeOut(b);
	fEdit(fmt);
}
void ZTextEdit::fUrl(bool b) {
	if (b)
		return;
	auto fmt = QTextCharFormat();
	fmt.setAnchor(false);
	fmt.setFontUnderline(false);
	fmt.setForeground(sample.foreground());
	fmt.setAnchorHref("");
	fmt.setAnchorName("");
	fEdit(fmt);
}

void ZTextEdit::pSplitline() {
	auto cursor = textCursor();
	cursor.beginEditBlock();
	cursor.insertHtml("<hr style=\"border-width:2px\"><br />");
	cursor.deletePreviousChar();
	cursor.endEditBlock();
}
void ZTextEdit::pInsImage() {
	auto cursor = textCursor();
	auto imageName = QFileDialog::getOpenFileName(this, "", "", "Images(*.jpg *.png *.svg *.tif *.bmp)");
	//    cursor.insertImage(QImage(imageName));
	cursor.insertHtml("<img src=\"" + imageName + "\" alt=\"" + imageName + "\"/>");
}
void ZTextEdit::pInsUrl(bool b) {
	if (!b)
		return;
	auto cursor = textCursor();
	cursor.beginEditBlock();
	auto bak = cursor.charFormat();
	cursor.insertHtml("<span>" + getUrlString() + "</span>");
	setCurrentCharFormat(bak);
	cursor.endEditBlock();
	updateCharFormat(currentCharFormat());
}
void ZTextEdit::pInsList() {
	auto cursor = textCursor();
	cursor.beginEditBlock();
	QTextListFormat fmt = QTextListFormat();
	fmt.setStyle(QTextListFormat::ListCircle);
	cursor.insertList(fmt);
	cursor.endEditBlock();
}
void ZTextEdit::pInsOrderedList() {
	auto cursor = textCursor();
	cursor.beginEditBlock();
	QTextListFormat fmt = QTextListFormat();
	fmt.setStyle(QTextListFormat::ListDecimal);
	cursor.insertList(fmt);
	cursor.endEditBlock();
}
void ZTextEdit::pInsCode() {
	auto cursor = textCursor();
	cursor.beginEditBlock();
	QTextFrameFormat framefmt;
	framefmt.setBorder(2);
	cursor.insertFrame(framefmt);
	cursor.insertText(DInputDialog::getMultiLineText(this, "Enter Your Code", "code"));
	cursor.endEditBlock();
}
QString ZTextEdit::getUrlString() {
	auto res = mDialog::getMultiText("Enter the Url", QList<QString>({ "name", "url" }));
	if (!res[1].contains("http"))
		res[1] = "http://" + res[1];
	return "<a href=\"" + res[1] + "\">" + res[0] + "</a>";
}
void ZTextEdit::updateCharFormat(const QTextCharFormat &nwfmt) {
	if (textCursor().hasSelection())
		return;
	emit fBoldState(nwfmt.fontWeight() >= QFont::DemiBold);
	emit fItalicState(nwfmt.fontItalic());
	emit fUnderlineState(nwfmt.fontUnderline());
	emit fLinethroughState(nwfmt.fontStrikeOut());
	emit pIsUrl(nwfmt.isAnchor());
}
void ZTextEdit::setHtml(const QString &html) {
	DTextEdit::setHtml(html);
	updateDocumentFormat();
}
void ZTextEdit::setBottomMargin(int d) {
	marginBottom = d;
	{ // init document format
		QTextFrameFormat fmt;
		fmt.setBottomMargin(marginBottom);
		document()->rootFrame()->setFrameFormat(fmt);
	}
}
void ZTextEdit::setTopMargin(int d) {
	marginTop = d;
}
void ZTextEdit::updateDocumentFormat() {
	QTextFrameFormat fmt;
	fmt.setBottomMargin(marginBottom);
	fmt.setTopMargin(marginTop);
	document()->rootFrame()->setFrameFormat(fmt);
}
