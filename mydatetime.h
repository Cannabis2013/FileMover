#ifndef MYDATETIME_H
#define MYDATETIME_H

#include <QDateTime>
#include <QDateEdit>
#include <QDate>

class myDateTime : public QDateTime
{
public:

    // Constructor..

    myDateTime(QDateTime dTime);
    myDateTime();
    //  Operators..
    bool operator <(myDateTime compared);
    bool operator >(myDateTime compared);
    bool operator <(QDateTime other);
    bool operator >(QDateTime other);
    bool operator ==(myDateTime other);
    bool operator ==(QDateTime other);
    void operator <<(const QDateTime &dTime);
    void operator <<(const QDateEdit *dEdit);
    void operator =(QDateTime dTime);
    void operator =(myDateTime dTime);
};

#endif // MYDATETIME_H
