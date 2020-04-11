#ifndef ENTITYDELEGATEBUILDER_H
#define ENTITYDELEGATEBUILDER_H

#include "entitymodel.h"
#include "imodeldelegates.h"
#include "idelegatebuilder.h"

class EntityDelegateBuilder : public IDelegateBuilder<DefaultDelegateModel,DefaultFileModelList>
{
public:
    DefaultDelegateModel *buildFileInformationDelegate(const QStringList &paths)
    {
        auto model = new FileInformationEntity;

    }
    DefaultDelegateModel *buildDirectoryCountDelegate(const QString &path, const QString &name, const long long &size)
    {

    }
    DefaultDelegateModel *buildFileRuleDelegate(const QStringList &paths, const DefaultFileModelList &files, int ruleAction, const QStringList &destinations)
    {

    }
    DefaultDelegateModel *buildErrorDelegate(const QString &errorMessage)
    {

    }
};

#endif // ENTITYDELEGATEBUILDER_H
