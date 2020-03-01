#ifndef MYICON_H
#define MYICON_H

#include <qicon.h>
#include <qstring.h>
#include "abstracticon.h"


class Icon :
        public AbstractIcon
{
public:
    ~Icon()
    {

    }
    Icon(QString fName = QString()):
        QIcon(fName)
    {
        addFile(fName);
    }

    void setName(const QString &n){N = n;}
    QString name() const{return N;}
private:
    QString N;
};

#endif // MYICON_H
