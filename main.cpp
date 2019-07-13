#include "mainwindow.h"
#include <qapplication.h>
#include <iostream>

using namespace std;

struct argumentValues
{
    QString orgName = "MH";
    QString appName = "Filehandler";
    bool testSession = false;
};

void processArguments(const QStringList &arguments,argumentValues &argVals)
{
    for (int i = 0;i <arguments.count();i++) {
        QString argument = arguments.value(i);
        if(argument == "--test-session")
            argVals.testSession = true;
        if(argument == "--application-name")
            argVals.appName = arguments.value(++i);
        if(argument == "--organization-name")
            argVals.appName = arguments.value(++i);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    argumentValues argVals;
    QStringList arguments = QApplication::arguments();

    processArguments(arguments,argVals);

    MainApplication *mainApplicaton = new MainApplication(argVals.appName,
                                                          argVals.orgName,
                                                          argVals.testSession);

    mainWindow *w = new mainWindow(mainApplicaton,
                                   argVals.appName,
                                   argVals.orgName);
    w->show();

    return a.exec();
}
