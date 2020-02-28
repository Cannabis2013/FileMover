#ifndef MYICON_H
#define MYICON_H

#include <qicon.h>
#include <qstring.h>

class AbstractIcon : public virtual QIcon
{
public:
    virtual void setName(const QString &n) = 0;
    virtual QString name() const = 0;
};

class Icon :
        public AbstractIcon
{
public:
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
