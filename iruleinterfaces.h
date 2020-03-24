#ifndef IRULEINTERFACES_H
#define IRULEINTERFACES_H

#include <qdatetime.h>

template<typename TSize,typename TSizeIntervals,typename TDateObject>
class IRuleCondition
{
public:
    virtual ~IRuleCondition() = default;
    virtual bool matchWholeWords() const = 0;
    virtual void setMatchWholeWords(bool matchWholeWords) = 0;
    virtual TSize sizeLimit() const = 0;
    virtual void setSizeLimit(const TSize &sizeLimit) = 0;
    virtual TSizeIntervals sizeInterval() const = 0;
    virtual void setSizeInterval(const TSizeIntervals &sizeInterval) = 0;
    virtual TDateObject date() const = 0;
    virtual void setDate(const TDateObject &date) = 0;
    virtual QPair<TDateObject, TDateObject> dateIntervals() const = 0;
    virtual void setDateIntervals(const QPair<TDateObject, TDateObject> &dateIntervals) = 0;
    virtual QStringList keyWords() const = 0;
    virtual void setKeyWords(const QStringList &keyWords) = 0;
    virtual int copyMode() const = 0;
    virtual void setCopyMode(const int &copymode) = 0;
    virtual int compareCriteria() const = 0;
    virtual void setCompareCriteria(const int &compareCriteria) = 0;
    virtual int criteria() const = 0;
    virtual void setCriteria(const int &criteria) = 0;
};

template<class TConditon>
class IRule
{
public:
    virtual ~IRule() = default;
    virtual int typeFilter() const = 0;
    virtual void setTypeFilter(const int &typeFilter) = 0;
    virtual int actionRuleEntity() const = 0;
    virtual void setActionRuleEntity(const int &actionRuleEntity) = 0;
    virtual QString title() const = 0;
    virtual void setTitle(const QString &title) = 0;
    virtual QStringList destinationPaths() const = 0;
    virtual void setDestinationPaths(const QStringList &destinationPaths) = 0;
    virtual QString appliesToPath() const = 0;
    virtual void setAppliesToPath(const QString &appliesToPath) = 0;
    virtual bool deepScanMode() const = 0;
    virtual void setDeepScanMode(bool deepScanMode) = 0;
    virtual QList<const TConditon*> conditions() const = 0;
    virtual void setCriterias(const QList<const TConditon*> &subRules) = 0;
};

typedef QPair<quint64,QString> SizeLimit;
typedef QPair<SizeLimit,SizeLimit> SizeLimits;
typedef QPair<QDateTime,QDateTime> DateInterval;

typedef IRuleCondition<SizeLimit,SizeLimits,QDateTime> IDefaultRuleCondition;
typedef IRule<IDefaultRuleCondition> IDefaultRule;


#endif // IRULEINTERFACES_H
