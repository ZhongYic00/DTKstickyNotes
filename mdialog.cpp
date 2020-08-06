#include "mdialog.h"

mDialog::mDialog(QWidget *p):DDialog(p)
{
}
QList<QString> mDialog::getMultiText(const QString &title,const QList<QString> &labels)
{
    mDialog dlg;
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(300);
    dlg.setWindowFlags(Qt::WindowCloseButtonHint|dlg.windowFlags());
    auto form=new QFormLayout(&dlg);
    QList<DLineEdit*> data;
    for(auto i:labels)
    {
        auto input=new DLineEdit(&dlg);
        form->addRow(i,input);
        data.push_back(input);
    }
    auto box=new QWidget(&dlg);
    box->setLayout(form);
    dlg.addContent(box);
    dlg.addButton("OK",false,DDialog::ButtonNormal);
    data[0]->setFocusPolicy(Qt::StrongFocus);
    dlg.exec();
    QList<QString> ret;
    for(auto i:data)
        ret.push_back(i->text());
    return ret;
}
