#include "mainwindow.h"
#include "myapp.h"
#include "argumentvalues.h"

int main(int argc, char *argv[])
{
    myApp a(argc, argv);
    QCoreApplication::setApplicationName("Filehandler");
    QCoreApplication::setOrganizationName("MH");

    a.setQuitOnLastWindowClosed(false);

    MainApplication *mainApplicaton = new MainApplication("Filehandler", "MH");

    mainWindow *w = new mainWindow(mainApplicaton,
                                   mainApplicaton->ApplicationTitle(),
                                   mainApplicaton->OrganisationTitle());

    QObject::connect(w,SIGNAL(quit(bool)),&a,SLOT(QuiT(bool)));

    w->show();

    return a.exec();
}
