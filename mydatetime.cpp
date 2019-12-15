#include "mydatetime.h"

myDateTime::myDateTime(QDateTime dTime)
{
    setDate(dTime.date());
    setTime(dTime.time());
}

myDateTime::myDateTime(int day, int month, int year, int hour, int minute, int seconds)
{
    QDate date(year,month,day);
    QTime time;
    time.setHMS(hour,minute,seconds);

    setDate(date);
    setTime(time);
}

myDateTime::myDateTime()
{
}

bool myDateTime::operator <(myDateTime compared)
{
    if(date().year() > compared.date().year())
        return true;
     if(date().year() < compared.date().year())
        return false;
    if(date().month() > compared.date().month())
        return true;
    else if(date().month() < compared.date().month())
        return false;
    if(date().day() > compared.date().day())
        return true;
    else if(date().day() <= compared.date().day())
        return false;
    else
        return false;
}

bool myDateTime::operator >(myDateTime compared)
{
    if(date().year() < compared.date().year())
        return true;
    else if(date().year() > compared.date().year())
        return false;
    if(date().month() < compared.date().month())
        return true;
    else if(date().month() > compared.date().month())
        return false;
    if(date().day() < compared.date().day())
        return true;
    else if(date().day() >= compared.date().day())
        return false;
    else
        return false;
}

bool myDateTime::operator <(QDateTime other)
{
    if(date().year() > other.date().year())
        return true;
    if(date().year() < other.date().year())
        return false;
    if(date().month() > other.date().month())
        return true;
    else if(date().month() < other.date().month())
        return false;
    if(date().day() > other.date().day())
        return true;
    else if(date().day() <= other.date().day())
        return false;
    else
        return false;
}


bool myDateTime::operator >(QDateTime other)
{
    QDate otherDate = other.date();
    if(date().year() < otherDate.year())
        return true;
    else if(date().year() > otherDate.year())
        return false;
    if(date().month() < otherDate.month())
        return true;
    else if(date().month() > otherDate.month())
        return false;
    if(date().day() < otherDate.day())
        return true;
    else if(date().day() >= otherDate.day())
        return false;
    else
        return false;
}

bool myDateTime::operator ==(myDateTime other)
{
    QDate otherDate = other.date();
    return date().year() == otherDate.year() &&
            date().month() == otherDate.month() &&
            date().day() == otherDate.day();
}

bool myDateTime::operator ==(QDateTime other)
{
    QDate otherDate = other.date();
    return date().year() == otherDate.year() &&
            date().month() == otherDate.month() &&
            date().day() == otherDate.day();
}

void myDateTime::operator <<(const QDateTime &dTime)
{
    setDate(dTime.date());
    setTime(dTime.time());
}

void myDateTime::operator <<(const QDateEdit *dEdit)
{
    setDate(dEdit->date());
    setTime(dEdit->time());
}

void myDateTime::operator =(QDateTime dTime)
{
    setDate(dTime.date());
    setTime(dTime.time());
}

void myDateTime::operator =(myDateTime dTime)
{
    setDate(dTime.date());
    setTime(dTime.time());
}

