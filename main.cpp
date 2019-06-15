#include "mainwindow.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(true);

    QString applicationName = "Filehandler", organizationName = "MH";

    MainApplication *mainApplicaton = new MainApplication(applicationName, organizationName);

    mainWindow *w = new mainWindow(mainApplicaton,
                                   applicationName,
                                   organizationName);
    w->show();

    return a.exec();
}
