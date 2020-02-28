#ifndef INHERITEXCEPTIONDELEGATE_H
#define INHERITEXCEPTIONDELEGATE_H

#include <typeinfo>
#include <iostream>
#include <qstring.h>

using namespace std;
template<typename TBase, typename TInherit>
class InheritExceptionDelegate : public exception
{
public:
    InheritExceptionDelegate()
    {
        QString base =  QString::fromLocal8Bit( typeid(TBase).name());
        QString derivative = QString::fromLocal8Bit(typeid (TInherit).name());

        msg = QString("Derivative of type %1 is not direct base of type %2").arg(derivative).arg(base).toLocal8Bit();
    }

    // exception interface
public:
    const char *what() const noexcept
    {
        return msg;
    }
    const char* msg;
};

#endif // INHERITEXCEPTIONDELEGATE_H
