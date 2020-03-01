#ifndef ABSTRACTICON_H
#define ABSTRACTICON_H

#include <qicon.h>

class AbstractIcon : public virtual QIcon
{
public:
    virtual ~AbstractIcon() = default;
    virtual void setName(const QString &n) = 0;
    virtual QString name() const = 0;
};

#endif // ABSTRACTICON_H
