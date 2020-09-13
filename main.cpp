#include "systemtray.h"
#include <DAboutDialog>
#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
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
int main(int argc, char *argv[]) {
	DApplication::loadDXcbPlugin();
	DApplication app(argc, argv);

#ifndef RUBBISH_DBG
	qInstallMessageHandler(myMessageOutput);
	std::freopen((QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/stickyNotes.log").toStdString().c_str(), "a", stderr);
	std::cerr << std::endl << std::endl;
#endif

	app.setAttribute(Qt::AA_UseHighDpiPixmaps);
	if (!app.setSingleInstance("DStickyNotes-develop"))
		return 0;
	app.setProductName("便笺");
	app.setProductIcon(QIcon(":/images/logo.svg.png"));
	app.setOrganizationName("RubbishTech");

	Systemtray tray(&app);
	tray.show();

	auto dlg = new DAboutDialog;
	dlg->setProductName("便笺");
	dlg->setProductIcon(QIcon(":/images/logo.svg.png"));
	dlg->setVersion("0.0.4-beta.1");
	dlg->setDescription(QWidget::tr("基于DTK开发的linux桌面便笺工具"));
	dlg->setWebsiteName("By Rubbish_ZhYic");
	dlg->setWebsiteLink("https://github.com/zhongyic00/");
	QIcon companyLogo(":/images/ZhYiclogo_v2");
	dlg->setCompanyLogo(companyLogo.pixmap(100, 100));
	app.setAboutDialog(dlg);

	int rt = app.exec();
	tray.save();
	return rt;
}
