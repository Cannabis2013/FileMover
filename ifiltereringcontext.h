#ifndef IRULEPROCESSOR_H
#define IRULEPROCESSOR_H

#include <qlist.h>
template<class TObject,class TSubject>
class IFiltereringContext
{
public:
    virtual QList<TSubject*> process(const TObject object, const QList<TSubject*> subjects) = 0;
};

#endif // IRULEPROCESSOR_H
