#ifndef MYICON_H
#define MYICON_H

#include <qicon.h>
#include <qstring.h>

class MyIcon : public QIcon
{
public:
    MyIcon(QString fName = QString()):
        QIcon(fName)
    {
        addFile(fName);
    }

    void setName(QString name){_name = name;}
    QString name() const{return _name;}
private:
    QString _name;
};

#endif // MYICON_H
