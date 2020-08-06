#include "ztextedit.h"
#include "mdialog.h"
#include <QDebug>
#include <DInputDialog>

ZTextEdit::ZTextEdit(QWidget* parent):DTextEdit(parent),ctrlPressed(false)
{
    QPalette p=this->palette();
    p.setBrush(QPalette::Dark,QBrush(QColor(130,130,130)));
    setPalette(p);
    setTextInteractionFlags(textInteractionFlags()|Qt::LinksAccessibleByMouse);
    connect(this,&DTextEdit::currentCharFormatChanged,this,&ZTextEdit::updateCharFormat);
    sample=textCursor().charFormat();
}

void ZTextEdit::keyPressEvent(QKeyEvent *e)
{

    if(e->key()==Qt::Key_Control)ctrlPressed=true;DTextEdit::keyPressEvent(e);
}
void ZTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Control)ctrlPressed=false;DTextEdit::keyPressEvent(e);
}
void ZTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    if(ctrlPressed)
    {
        auto cursor=textCursor();
        if(cursor.charFormat().isAnchor())
            QDesktopServices::openUrl(QUrl(cursor.charFormat().anchorHref()));
    }
    QWidget::mousePressEvent(e);
}

void ZTextEdit::insertFromMimeData(const QMimeData *src)
{
    if(!src->hasHtml()&&src->hasImage())DTextEdit::insertFromMimeData(src);
    else
    {
        auto cursor=textCursor();
        cursor.beginEditBlock();
        auto bak=cursor.charFormat();
        auto blkbak=cursor.blockFormat();
        bak.setProperty(QTextCharFormat::FontPixelSize,-1);
        auto fmt=QTextCharFormat(bak);
        fmt.clearProperty(QTextCharFormat::FontWeight);
        fmt.clearProperty(QTextCharFormat::FontItalic);
        fmt.clearProperty(QTextCharFormat::FontUnderline);
        fmt.clearProperty(QTextCharFormat::FontStrikeOut);
        fmt.setProperty(QTextCharFormat::FontPixelSize,-1);
        fmt.setForeground(bak.foreground());
        fmt.setBackground(bak.background());
        fmt.setAnchor(false);
        fmt.setAnchorHref("");
        auto blkfmt=QTextBlockFormat();
        blkfmt.setBackground(bak.background());
        blkfmt.setProperty(QTextBlockFormat::FontPixelSize,-1);
        blkfmt.setProperty(QTextBlockFormat::LineHeight,blkbak.property(QTextBlockFormat::LineHeight));
        if(cursor.hasSelection())cursor.removeSelectedText();
        int uninsertedPos=cursor.position();
        if(!uninsertedPos)cursor.insertText(" ");
        cursor.insertHtml(src->html());
        int insertedPos=cursor.position();
        cursor.setPosition(uninsertedPos,QTextCursor::MoveAnchor);
        cursor.setPosition(insertedPos,QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(fmt);
        cursor.mergeBlockFormat(blkfmt);
        cursor.setPosition(insertedPos,QTextCursor::MoveAnchor);
        setTextBackgroundColor(bak.background().color());
        if(!uninsertedPos)//在空白textEdit中粘贴文本会，然后全选删除，会出现原因不明的光标下格式错误。
        {
            QTextCursor csr(cursor);
            csr.setPosition(0);
            csr.deleteChar();
        }
        setCurrentCharFormat(bak);
        cursor.endEditBlock();
    }
}

inline void ZTextEdit::fEdit(QTextCharFormat &fmt)
{
    auto selection=textCursor();
    selection.beginEditBlock();
    if(!selection.hasSelection())mergeCurrentCharFormat(fmt);
    else selection.mergeCharFormat(fmt);
    selection.endEditBlock();
}
void ZTextEdit::fBold(bool b)
{
    auto fmt=QTextCharFormat();
    fmt.setFontWeight(b?QFont::DemiBold:QFont::Normal);
    fEdit(fmt);
}
void ZTextEdit::fItalic(bool b)
{
    auto fmt=QTextCharFormat();
    fmt.setFontItalic(b);
    fEdit(fmt);
}
void ZTextEdit::fUnderline(bool b)
{
    auto fmt=QTextCharFormat();
    fmt.setFontUnderline(b);
    fEdit(fmt);
}
void ZTextEdit::fLinethrough(bool b)
{
    auto fmt=QTextCharFormat();
    fmt.setFontStrikeOut(b);
    fEdit(fmt);
}
void ZTextEdit::fUrl(bool b)
{
    if(b)return ;
    auto fmt=QTextCharFormat();
    fmt.setAnchor(false);
    fmt.setFontUnderline(false);
    fmt.setForeground(sample.foreground());
    fmt.setAnchorHref("");
    fmt.setAnchorName("");
    fEdit(fmt);
}

void ZTextEdit::pSplitline()
{
    auto cursor=textCursor();
    cursor.beginEditBlock();
    cursor.insertHtml("<hr style=\"border-width:2px\"><br />");
    cursor.deletePreviousChar();
    cursor.endEditBlock();
}
void ZTextEdit::pInsImage()
{
    auto cursor=textCursor();
    cursor.insertImage(QImage(QFileDialog::getOpenFileName(this,"","","Images(*.jpg *.png *.svg *.tif *.bmp)")));
}
void ZTextEdit::pInsUrl(bool b)
{
    if(!b)return ;
    auto cursor=textCursor();
    cursor.beginEditBlock();
    auto bak=cursor.charFormat();
    cursor.insertHtml("<span>"+getUrlString()+"</span>");
    setCurrentCharFormat(bak);
    cursor.endEditBlock();
    updateCharFormat(currentCharFormat());
}
void ZTextEdit::pInsList()
{
    auto cursor=textCursor();
    cursor.beginEditBlock();
    QTextListFormat fmt=QTextListFormat();
    fmt.setStyle(QTextListFormat::ListCircle);
    cursor.insertList(fmt);
    cursor.endEditBlock();
}
void ZTextEdit::pInsOrderedList()
{
    auto cursor=textCursor();
    cursor.beginEditBlock();
    QTextListFormat fmt=QTextListFormat();
    fmt.setStyle(QTextListFormat::ListDecimal);
    cursor.insertList(fmt);
    cursor.endEditBlock();
}
void ZTextEdit::pInsCode()
{
    auto cursor=textCursor();
    cursor.beginEditBlock();
    QTextFrameFormat framefmt;
    framefmt.setBorder(2);
    cursor.insertFrame(framefmt);
    cursor.insertText(DInputDialog::getMultiLineText(this,"Enter Your Code","code"));
    cursor.endEditBlock();
}
QString ZTextEdit::getUrlString()
{
    auto res=mDialog::getMultiText("Enter the Url",QList<QString>({"name","url"}));
    if(!res[1].contains("http"))res[1]="http://"+res[1];
    return "<a href=\""+res[1]+"\">"+res[0]+"</a>";
}
void ZTextEdit::updateCharFormat(const QTextCharFormat& nwfmt)
{
    if(textCursor().hasSelection())return ;
    emit fBoldState(nwfmt.fontWeight()>=QFont::DemiBold);
    emit fItalicState(nwfmt.fontItalic());
    emit fUnderlineState(nwfmt.fontUnderline());
    emit fLinethroughState(nwfmt.fontStrikeOut());
    emit pIsUrl(nwfmt.isAnchor());
/*    if(nwfmt.background()!=sample.background())
    {
        qDebug()<<"ERR";
        auto fmt=QTextCharFormat();
        fmt.setBackground(sample.background());
        textCursor().mergeCharFormat(fmt);
        setTextBackgroundColor(QColor("#E5E5E5"));
    }*/
}
