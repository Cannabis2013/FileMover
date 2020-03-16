#ifndef IRULEDEFINITIONS_H
#define IRULEDEFINITIONS_H

#include <qlist.h>

template<typename TRuleType, typename TRuleAction,typename TRuleCriteria,typename TRuleCompareCriteria, typename TFileEntity>
class IRuleDefinitions
{
public:

    virtual const QStringList intervalConditionalList() = 0;
    virtual const QList<QPair<QString,TRuleAction> > actionMappings()=0;
    virtual const QList<QPair<QString,TRuleCriteria> > criteriaMappings()=0;
    virtual const QList<QPair<QString,TRuleCompareCriteria> > compareCriteriaMappings() = 0;
    virtual const QList<QPair<QString,TFileEntity>> fileTypeFilterMappings() = 0;

    virtual const QStringList buildStringListFromEntity(TRuleType property) = 0;
    virtual const QStringList fileCompareModesToStringList(TRuleCriteria condition) = 0;
    virtual const QStringList allFileTypeEntitiesToStrings() = 0;

    virtual QString fileActionEntityToString(const TRuleAction mode) = 0;
    virtual TRuleAction fileActionEntityFromString(const QString mode) = 0;
    virtual QString buildStringFromCriteria(const TRuleCriteria mode) = 0;
    virtual TRuleCriteria buildCriteriaFromString(const QString string) = 0;
    virtual QString buildStringFromCompareCriteria(const TRuleCompareCriteria mode) = 0;
    virtual TRuleCompareCriteria fileCompareEntityFromString(const QString string) = 0;
    virtual QString fileTypeEntityToString(TFileEntity type) = 0;
    virtual TFileEntity fileTypeEntityFromString(const QString &string) = 0;

    virtual QStringList sizeUnits() = 0;
    virtual QString buildDefaultStringValue() = 0;
};

#endif // IRULEDEFINITIONS_H
