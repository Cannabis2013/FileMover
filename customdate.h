#ifndef MYDATETIME_H
#define MYDATETIME_H

#include <QDateTime>
#include <QDateEdit>
#include <QDate>

/*
 * Compare operators related:
 *  - Bigger is older
 *  - Lesser is younger
 */

class CustomDate : public QDateTime
{
public:

    // Constructor..

    explicit CustomDate(const QDateTime &dTime);
    CustomDate(int day,
               int month,
               int year,
               int hour = 0,
               int minute = 0,
               int seconds = 0);
    CustomDate()
    {}
    //  Operators..
    bool operator <(CustomDate compared);
    bool operator >(CustomDate compared);
    bool operator <(QDateTime other);
    bool operator >(QDateTime other);
    bool operator ==(CustomDate other);
    bool operator ==(QDateTime other);
    CustomDate &operator <<(const QDateTime &dTime);
    CustomDate &operator <<(const QDateEdit *dEdit);
};

#endif // MYDATETIME_H
