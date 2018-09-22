#ifndef MYICON_H
#define MYICON_H

#include <qicon.h>
#include <qstring.h>

class myIcon : public QIcon
{
public:
    myIcon(QString fName = QString()):
        QIcon(fName)
    {
        addFile(fName);
    }
    void setName(QString n){N = n;}
    QString name() const{return N;}
private:
    QString N;
};

#endif // MYICON_H
