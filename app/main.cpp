#include <QApplication>

#include "fontend/mainwindow.h"

#include "backend/backendfactory.h"
#include "backend/winbackend.h"

#include "config/log.h"
#include "config/config.h"

#include <xsys.h>


#include <QApplication>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QProcess>
#include <Windows.h>
#include <QMessageBox>
#include <QThread>
#include <QMessageBox>

bool processArgs(QApplication &app) {
    QCoreApplication::setApplicationName("Deepin System Installer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    //parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();

    // A boolean option with multiple names (-u, --uninstall)
    QCommandLineOption uninstallOption(
                QStringList() << "u" << "uninstall",
                "uninstall install system.");
    parser.addOption(uninstallOption);

    // An option with a value
    QCommandLineOption targetDirectoryOption(
                QStringList() << "t" << "target-directory",
                "install directory.",
                "directory.");
    parser.addOption(targetDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    bool uninstall = parser.isSet(uninstallOption);
    QString targetDir = parser.value(targetDirectoryOption);

    if (uninstall) {
        QThread::sleep(4);
        Xapi::RmDir(targetDir);
    }
    return uninstall;
}

int main(int argc, char**argv) {
    Q_INIT_RESOURCE(xsys);
    Q_INIT_RESOURCE(dwidget);
    QApplication app(argc, argv);
    InstallLogHandler();

    if (processArgs(app)) {
        return 0;
    }

    ImmDisableIME(GetCurrentThreadId());

    LoadFonts();
    LoadTranslate(app);

    MainWindow w;
    w.setWindowTitle (DeepinInstaller::AppTitle());
    w.setWindowIcon(QIcon(":/data/deepin-system-installer.png"));
    w.show();

    int ret = app.exec();
    qDebug()<<"app.exec() return with"<<ret;
    return 0;
}
