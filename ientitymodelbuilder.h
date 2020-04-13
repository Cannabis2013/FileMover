#ifndef IDELEGATEBUILDER_H
#define IDELEGATEBUILDER_H

#include <qstringlist.h>

template<class TDelegateInterface, typename TFileModelList = QStringList>
class IEntityModelBuilder
{
public:
    virtual TDelegateInterface *buildFileInformationModel(const QStringList &paths) = 0;
    virtual TDelegateInterface *buildDirectoryCountModel(const QString &path, const QString &name,const long long &size) = 0;
    virtual TDelegateInterface *buildFileRuleModel(const QStringList &paths, const TFileModelList &files, int ruleAction, const QStringList &destinations) = 0;
    virtual TDelegateInterface *buildErrorModel(const QString &errorMessage) = 0;
};

#endif // IDELEGATEBUILDER_H
