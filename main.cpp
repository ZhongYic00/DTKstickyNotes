#include "daemon.h"
#include <DAboutDialog>
#include <DApplication>
#include <DApplicationSettings>
#include <DLog>
#include <DSettings>
#include <QDebug>

#ifdef RELEASE
#define APPLICATION_NAME "DTKStickyNotes"
#else
#define APPLICATION_NAME "DTKStickyNotes-develop"
#endif
//#define APP_VERSION "0.0.5-beta"

DWIDGET_USE_NAMESPACE
void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }
}
int main(int argc, char* argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);

#ifdef RELEASE
    qInstallMessageHandler(myMessageOutput);
    std::freopen((QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + APPLICATION_NAME + ".log").toStdString().c_str(), "a", stderr);
    std::cerr << std::endl
              << std::endl;
#endif
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationLicense("GPLv2");
    app.setApplicationHomePage("https://github.com/zhongyic00/DTKStickyNotes/");
    app.setApplicationDescription(QObject::tr("基于DTK开发的linux桌面便笺工具"));
    app.setApplicationAcknowledgementPage("https://github.com/zhongyic00/");
    app.setApplicationAcknowledgementVisible(true);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName("RubbishTech");
    app.setOrganizationDomain("https://github.com/zhongyic00/");
    app.setProductName(QObject::tr("深度便笺"));
    app.setProductIcon(QIcon(":/images/logo.svg.png"));
    /*QIcon companyLogo(":/images/ZhYiclogo_v2");
    app.aboutDialog()->setCompanyLogo(companyLogo.pixmap(100, 100));
    app.aboutDialog()->setWebsiteName("By Rubbish_ZhYic");*/

    if (!app.setSingleInstance(app.applicationName()))
        return 0;

    DApplicationSettings appSettings(&app); //initialize application theme settings

    Daemon daemon;

    /*DAboutDialog dlg;
    dlg.setProductName("便笺");
    dlg.setProductIcon(QIcon(":/images/logo.svg.png"));
    dlg.setVersion("0.0.4-beta.1");
    dlg.setDescription(QWidget::tr("基于DTK开发的linux桌面便笺工具"));
    dlg.setWebsiteName("By Rubbish_ZhYic");
    dlg.setWebsiteLink("https://github.com/zhongyic00/");
    QIcon companyLogo(":/images/ZhYiclogo_v2");
    dlg.setCompanyLogo(companyLogo.pixmap(100, 100));
    app.setAboutDialog(&dlg);*/

    return app.exec();
}
