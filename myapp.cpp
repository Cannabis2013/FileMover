#include "myapp.h"
#include "iostream"

using namespace std;

myApp::myApp(int ar, char **arc) :
    QApplication(ar,arc)
{
    readSettings();
}

void myApp::QuiT(bool h)
{
    start = h;
    writeSettings();
    quit();
}

void myApp::writeSettings()
{
    QSettings s("MH","Filehandler");

    s.beginGroup("Start Values");
    s.setValue("hideOnStart",QVariant(start));
    s.endGroup();
}

void myApp::readSettings()
{
    QSettings s("MH","Filehandler");
    s.beginGroup("Start Values");
    start = s.value("hideOnStart",QVariant(false)).toBool();
    s.endGroup();
}
