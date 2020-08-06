#include "mainwindow.h"
#include <DApplication>
#include <DAboutDialog>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc,argv);

    QFile qss(":/stylesheets/mainwindow.qss");
    qss.open(QFile::ReadOnly|QFile::Text);
    app.setStyleSheet(qss.readAll());
    qss.close();

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    if(!app.setSingleInstance("DStickyNotes"))return 0;
    app.setProductName("深度便签");
    app.setProductIcon(QIcon(":/images/logo.svg.png"));
    app.setOrganizationName("Rubbishtech Co.,Ltd");

    auto backend=new ZBackend;
    MainWindow win(backend);

    auto dlg=new DAboutDialog(&win);
    dlg->setProductName("便笺");
    dlg->setProductIcon(QIcon(":/images/logo.svg.png"));
    dlg->setVersion("0.0.1");
    dlg->setDescription(QWidget::tr("基于DTK开发的linux桌面便笺工具"));
    dlg->setWebsiteName("By Rubbish_ZhYic");
    dlg->setWebsiteLink("https://github.com/zhongyic00/");
    QIcon companyLogo(":/images/ZhYiclogo_v2");
    dlg->setCompanyLogo(companyLogo.pixmap(100,100));
    app.setAboutDialog(dlg);

    win.move(DApplication::desktop()->screenGeometry().center()-QPoint(win.width()/2,win.height()/2));
    win.show();

    int rt=app.exec();
    win.save();
    return rt;
}
