#ifndef IDELEGATEBUILDER_H
#define IDELEGATEBUILDER_H

#include <qstringlist.h>

template<class TDelegateInterface, typename TFileModelList = QStringList>
class IDelegateBuilder
{
public:
    virtual TDelegateInterface *buildFileInformationDelegate(const QStringList &paths) = 0;
    virtual TDelegateInterface *buildDirectoryCountDelegate(const QString &path, const QString &name,const long long &size) = 0;
    virtual TDelegateInterface *buildFileRuleDelegate(const QStringList &paths, const TFileModelList &files, int ruleAction, const QStringList &destinations) = 0;
    virtual TDelegateInterface *buildErrorDelegate(const QString &errorMessage) = 0;
};

#endif // IDELEGATEBUILDER_H
