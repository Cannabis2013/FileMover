#ifndef MYAPP_H
#define MYAPP_H

#include <QApplication>
#include <qsettings.h>

class myApp : public QApplication
{
    Q_OBJECT
public:
    explicit myApp(int ar, char **arc);
    bool hideOnStart() const{return start;}

signals:

public slots:
    void QuiT(bool h);

private:
    void writeSettings();
    void readSettings();

    bool start;
};

#endif // MYAPP_H
