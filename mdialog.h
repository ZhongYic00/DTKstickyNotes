#ifndef MDIALOG_H
#define MDIALOG_H

//#include <DInputDialog>
#include <DLineEdit>
#include <DButtonBox>
#include <QtWidgets>
#include <DDialog>
//#include <DtkWidgets>

DWIDGET_USE_NAMESPACE
class mDialog:public DDialog
{
    Q_OBJECT
public:
    mDialog(QWidget *p=nullptr);
    static QList<QString> getMultiText(const QString &title,const QList<QString> &labels);
};

#endif // MDIALOG_H
