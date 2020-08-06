#ifndef ZTEXTEDIT_H
#define ZTEXTEDIT_H
#include <DTextEdit>
#include <QtWidgets>

DWIDGET_USE_NAMESPACE
class ZTextEdit:public DTextEdit
{
    Q_OBJECT
public:
    ZTextEdit(QWidget* p);
public slots:
    void fBold(bool);
    void fItalic(bool);
    void fUnderline(bool);
    void fLinethrough(bool);
    void fUrl(bool);
    void pSplitline();
    void pInsImage();
    void pInsUrl(bool);
    void pInsList();
    void pInsOrderedList();
    void pInsCode();
    QString getUrlString();
    void updateCharFormat(const QTextCharFormat&);
signals:
    void fBoldState(bool);
    void fItalicState(bool);
    void fUnderlineState(bool);
    void fLinethroughState(bool);
    void pIsUrl(bool);
protected:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void insertFromMimeData(const QMimeData *);
private:
    void fEdit(QTextCharFormat&);
    QTextCharFormat sample;
    bool ctrlPressed;
    bool editing;
};

#endif // ZTEXTEDIT_H
