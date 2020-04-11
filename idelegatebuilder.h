#ifndef IDELEGATEBUILDER_H
#define IDELEGATEBUILDER_H

#include <qstringlist.h>

template<class TModelInterface, typename TFileModelList = QStringList>
class IDelegateBuilder
{
public:
    virtual TModelInterface *buildFileInformationDelegate(const QStringList &paths) = 0;
    virtual TModelInterface *buildDirectoryCountDelegate(const QString &path, const QString &name,const long long &size) = 0;
    virtual TModelInterface *buildFileRuleDelegate(const QStringList &paths, const TFileModelList &files, int ruleAction, const QStringList &destinations) = 0;
    virtual TModelInterface *buildErrorDelegate(const QString &errorMessage) = 0;
};

#endif // IDELEGATEBUILDER_H
