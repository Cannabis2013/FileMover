#ifndef IRULECONFIGURATION_H
#define IRULECONFIGURATION_H

#include <qstringlist.h>

template<class TCondition,typename TFileType,typename TFileAction>
class IRuleConfiguration
{
public:
    virtual QString appliesTo() const = 0;
    virtual void setAppliesTo(const QString &appliesTo) = 0;
    virtual QStringList destinations() const = 0;
    virtual void setDestinations(const QStringList &destinations) = 0;
    virtual TFileAction action() const = 0;
    virtual void setAction(const TFileAction &action) = 0;
    virtual TFileType type() const = 0;
    virtual void setType(const TFileType &type) = 0;
    virtual QList<const TCondition*> conditions() const = 0;
    virtual void setConditions(const QList<const TCondition*> &conditions) = 0;
    virtual bool deepScanMode() const = 0;
    virtual void setDeepScanMode(bool deepScanMode) = 0;
    virtual QString title() const = 0;
    virtual void setTitle(const QString &title) = 0;
};
template<typename TCriteria,typename TComparisonCriteria,typename TSizeContainer,typename TSizeIntervals,typename TDate , typename TDateIntervals>
class IRuleConditionConfiguration
{
public:
    virtual TCriteria criteria() const = 0;
    virtual void setCriteria(const TCriteria &criteria) = 0;
    virtual TComparisonCriteria compareCriteria() const = 0;
    virtual void setCompareCriteria(const TComparisonCriteria &compareCriteria) = 0;
    virtual QStringList keywords() const = 0;
    virtual void setKeywords(const QStringList &value) = 0;
    virtual TSizeContainer sizeLimit() const = 0;
    virtual void setSizeLimit(const TSizeContainer &sizeLimit) = 0;
    virtual TDate date() const = 0;
    virtual void setDate(const TDate &date) = 0;
    virtual TSizeIntervals sizeInterval() const = 0;
    virtual void setSizeInterval(const TSizeIntervals &sizeInterval) = 0;
    virtual TDateIntervals dates() const = 0;
    virtual void setDates(const TDateIntervals &dates) = 0;
    virtual bool matchWholeWords() const = 0;
    virtual void setMatchWholeWords(bool matchWholeWords) = 0;
};

#endif // IRULECONFIGURATION_H
