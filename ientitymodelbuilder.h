#ifndef IDELEGATEBUILDER_H
#define IDELEGATEBUILDER_H

#include <qstringlist.h>

template<class TEntityModelInterface, typename TFileModelList = QStringList>
class IEntityModelBuilder
{
public:
    virtual TEntityModelInterface *buildFileInformationModel(const QStringList &paths) = 0;
    virtual TEntityModelInterface *buildDirectoryCountModel(const QString &path, const QString &name,const long long &size) = 0;
    virtual TEntityModelInterface *buildFileRuleModel(const TFileModelList &files, int ruleAction, const QStringList &destinations) = 0;
    virtual TEntityModelInterface *buildErrorModel(const QString &errorMessage) = 0;
};

#endif // IDELEGATEBUILDER_H
