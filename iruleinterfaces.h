#ifndef IRULEINTERFACES_H
#define IRULEINTERFACES_H

#include <qdatetime.h>
#include "TypeDefinitions.h"

typedef QPair<quint64,QString> SizeLimit;
typedef QPair<SizeLimit,SizeLimit> SizeLimits;
typedef QPair<QDateTime,QDateTime> DateInterval;

class IRuleCondition
{
public:
    virtual ~IRuleCondition() = default;
    virtual bool matchWholeWords() const = 0;
    virtual void setMatchWholeWords(bool matchWholeWords) = 0;
    virtual SizeLimits sizeInterval() const = 0;
    virtual void setSizeInterval(const SizeLimits &sizeInterval) = 0;
    virtual QDateTime date() const = 0;
    virtual void setDate(const QDateTime &date) = 0;
    virtual QPair<QDateTime, QDateTime> dateIntervals() const = 0;
    virtual void setDateIntervals(const QPair<QDateTime, QDateTime> &dateIntervals) = 0;
    virtual QStringList keyWords() const = 0;
    virtual void setKeyWords(const QStringList &keyWords) = 0;
    virtual RulesContext::CopyMode copyMode() const = 0;
    virtual void setCopyMode(const RulesContext::CopyMode &copymode) = 0;
    virtual RulesContext::RuleCompareCriteria compareCriteria() const = 0;
    virtual void setCompareCriteria(const RulesContext::RuleCompareCriteria &compareCriteria) = 0;
    virtual RulesContext::RuleCriteria criteria() const = 0;
    virtual void setCriteria(const RulesContext::RuleCriteria &criteria) = 0;
    virtual QPair<quint64, QString> sizeLimit() const = 0;
    virtual void setSizeLimit(const QPair<uint, QString> &sizeLimit) = 0;
};

template<class TConditon = IRuleCondition>
class IRule
{
public:
    virtual ~IRule() = default;
    virtual RulesContext::FileType typeFilter() const = 0;
    virtual void setTypeFilter(const RulesContext::FileType &typeFilter) = 0;
    virtual RulesContext::RuleAction actionRuleEntity() const = 0;
    virtual void setActionRuleEntity(const RulesContext::RuleAction &actionRuleEntity) = 0;
    virtual QString title() const = 0;
    virtual void setTitle(const QString &title) = 0;
    virtual QStringList destinationPaths() const = 0;
    virtual void setDestinationPaths(const QStringList &destinationPaths) = 0;
    virtual QString appliesToPath() const = 0;
    virtual void setAppliesToPath(const QString &appliesToPath) = 0;
    virtual bool deepScanMode() const = 0;
    virtual void setDeepScanMode(bool deepScanMode) = 0;
    virtual QList<const TConditon*> conditions() const = 0;
    virtual void setSubRules(const QList<const TConditon*> &subRules) = 0;
};

namespace RulesContext
{
    static QString ruleSizeLimitsToString(const IRuleCondition *sRule)
    {
        QString minSize = QString::number(sRule->sizeInterval().first.first),
                maxSize = QString::number(sRule->sizeInterval().second.first);
        QString sizeUnitMin = sRule->sizeInterval().first.second,
                sizeUnitMax = sRule->sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }

    static QString ruleDateLimitsToString(const IRuleCondition *sRule)
    {
        QString startDate = sRule->dateIntervals().first.date().toString("dd.MM.yyyy"),
                endDate = sRule->dateIntervals().second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }

    static QString mergeStringList(const QStringList strings)
    {
        if(strings.empty())
            return QString();
        else if(strings.count() == 1)
            return strings.first();

        QString result;
        for(QString string : strings)
            result += string + ";";
        return result;
    }

    static QStringList splitString(const QString split)
    {
        QString tempString;
        QStringList splittetList;
        int lastLetter = split.count() -1;
        for(int i = 0;i<split.count();i++)
        {
            QChar w = split.at(i);
            if(w != ';' && lastLetter != i)
                tempString.append(w);
            else
            {
                splittetList << tempString + w;
                tempString.clear();
            }
        }
        return splittetList;
    }

    static QString ruleKeyWordToString(const IRuleCondition *sRule)
    {
        if(sRule->criteria() == RulesContext::FileSize &&
                sRule->compareCriteria() != RulesContext::Interval)
            return QString::number(sRule->sizeLimit().first) + " " + sRule->sizeLimit().second;
        else if(sRule->criteria() == RulesContext::FileSize &&
                sRule->compareCriteria() == RulesContext::Interval)
            return RulesContext::ruleSizeLimitsToString(sRule);
        else if((sRule->criteria() == RulesContext::FileCreatedMode || sRule->criteria() == RulesContext::FileModifiedMode) &&
                sRule->compareCriteria() != RulesContext::Interval)
            return sRule->date().toString("dd.MM.yyyy");
        else if((sRule->criteria() == RulesContext::FileCreatedMode || sRule->criteria() == RulesContext::FileModifiedMode) &&
                sRule->compareCriteria() == RulesContext::Interval)
            return RulesContext::ruleDateLimitsToString(sRule);
        else
            return RulesContext::mergeStringList(sRule->keyWords());
    }
}

#endif // IRULEINTERFACES_H
