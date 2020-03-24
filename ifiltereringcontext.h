#ifndef IRULEPROCESSOR_H
#define IRULEPROCESSOR_H

#include <qlist.h>

template<class TObject,class TSubject, class TFileListServiceProvider>
class IFiltereringContext
{
public:
    virtual QList<TSubject*> process(const QList<TObject*> objects, const QList<TSubject*> subjects) = 0;
    virtual void setListService(TFileListServiceProvider *service) = 0;
    virtual TFileListServiceProvider *listService() = 0;
};

#endif // IRULEPROCESSOR_H
