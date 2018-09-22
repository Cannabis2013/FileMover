#include "mainwindow.h"
#include "myapp.h"
#include "argumentvalues.h"

int main(int argc, char *argv[])
{
    myApp a(argc, argv);

    QCoreApplication::setApplicationName("Filehandler");
    QCoreApplication::setOrganizationName("MH");

    a.setQuitOnLastWindowClosed(false);

    bool hidden = a.hideOnStart();

#if(__MINGW32__)
    altArgsContainer altArgs;

    QStringList argList = a.arguments();
    for(QString arg : argList)
    {
        if(arg == "Developer" || arg == "developer")
            altArgs.developerMode = true;
        else if(arg.startsWith("i") || arg.startsWith("I"))
        {
            arg.remove(0,1);
            altArgs.alternateIconPath = arg;
        }
    }
#endif

    mainWindow *w = new mainWindow(altArgs);

    QObject::connect(w,SIGNAL(quit(bool)),&a,SLOT(QuiT(bool)));
    if(!hidden)
        w->show();

    return a.exec();
}
