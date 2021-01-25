#ifndef ZTEXTEDIT_H
#define ZTEXTEDIT_H
#include "zlistmodel.h"
#include <DTextEdit>
#include <QtWidgets>

DWIDGET_USE_NAMESPACE
class ZTextEdit : public DTextEdit {
    Q_OBJECT
public:
    ZTextEdit(QWidget* p, bool DStyle = true);
    void setNote(const ZNote& note);
    inline InnerIndex index() const;
public slots:
    void fBold(bool);
    void fItalic(bool);
    void fUnderline(bool);
    void fLinethrough(bool);
    void fUrl(bool);
    void pSplitline();
    void pInsImage(const QVariant& data = QVariant());
    void pInsUrl(bool);
    void pInsList();
    void pInsOrderedList();
    void pInsCode();
    QString getUrlString();
    void updateCharFormat(const QTextCharFormat&);
    void setHtml(QString html);
    void setBottomMargin(int d);
    void setTopMargin(int d);
signals:
    void fBoldState(bool);
    void fItalicState(bool);
    void fUnderlineState(bool);
    void fLinethroughState(bool);
    void pIsUrl(bool);
    void indexChanged(const InnerIndex& ori, const InnerIndex& idx);

protected:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void insertFromMimeData(const QMimeData*);
    void resizeEvent(QResizeEvent*);
    bool event(QEvent* e);
private slots:

private:
    void fEdit(QTextCharFormat&);
    void updateDocumentFormat();
    void initResources();
    QImage processImage(const QByteArray& data);
    void updateResources();
    void updateResourcesList(QString& html);
    void commitModifications();

    QTextCharFormat sample;
    QStringList resources;
    bool ctrlPressed;
    bool editing;
    bool hackDStyle;
    int marginBottom;
    int marginTop;
    const double ImagePercentage = 0.95;
    InnerIndex idx;
};
InnerIndex ZTextEdit::index() const { return idx; }
#endif // ZTEXTEDIT_H
