#ifndef RULES_H
#define RULES_H

#include "ruledefinitions.h"
#include <QPair>
#include <QStringList>
#include "customdate.h"

typedef QPair<quint64,QString> SizeLimit;
typedef QPair<SizeLimit,SizeLimit> SizeLimits;
typedef QPair<CustomDate,CustomDate> DateInterval;

class SubRule
{
public:

    bool matchWholeWords() const
    {
        return _matchWholeWords;
    }
    void setMatchWholeWords(bool matchWholeWords)
    {
        _matchWholeWords = matchWholeWords;
    }

    SizeLimits sizeInterval() const
    {
        return _sizeInterval;
    }
    void setSizeInterval(const SizeLimits &sizeInterval)
    {
        _sizeInterval = sizeInterval;
    }

    CustomDate date() const
    {
        return _date;
    }
    void setDate(const CustomDate &date)
    {
        _date = date;
    }

    QPair<CustomDate, CustomDate> dateIntervals() const
    {
        return _dateIntervals;
    }
    void setDateIntervals(const QPair<CustomDate, CustomDate> &dateIntervals)
    {
        _dateIntervals = dateIntervals;
    }

    QStringList keyWords() const
    {
        return _keyWords;
    }
    void setKeyWords(const QStringList &keyWords)
    {
        _keyWords = keyWords;
    }

    RulesContext::CopyMode copyMode() const
    {
        return _copyMode;
    }
    void setCopyMode(const RulesContext::CopyMode &copymode)
    {
        _copyMode = copymode;
    }

    RulesContext::RuleCompareCriteria compareCriteria() const
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const RulesContext::RuleCompareCriteria &compareCriteria)
    {
        _compareCriteria = compareCriteria;
    }

    RulesContext::RuleCriteria criteria() const
    {
        return _criteria;
    }
    void setCriteria(const RulesContext::RuleCriteria &criteria)
    {
        _criteria = criteria;
    }

    QPair<quint64, QString> sizeLimit() const
    {
        return _sizeLimit;
    }
    void setSizeLimit(const QPair<uint, QString> &sizeLimit)
    {
        _sizeLimit = sizeLimit;
    }

private:
    RulesContext::CopyMode _copyMode = RulesContext::NoMode;
    RulesContext::RuleCompareCriteria _compareCriteria = RulesContext::noCompareModeSet;
    RulesContext::RuleCriteria _criteria = RulesContext::nonConditionalMode;
    QPair<quint64,QString>_sizeLimit;
    SizeLimits _sizeInterval;
    CustomDate _date;
    QPair<CustomDate,CustomDate> _dateIntervals;
    bool _matchWholeWords = false;
    QStringList _keyWords;
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

class Rule
{
public:
    RulesContext::FileType typeFilter() const
    {
        return _typeFilter;
    }
    void setTypeFilter(const RulesContext::FileType &typeFilter)
    {
        _typeFilter = typeFilter;
    }

    RulesContext::RuleAction actionRuleEntity() const
    {
        return _actionRuleEntity;
    }
    void setActionRuleEntity(const RulesContext::RuleAction &actionRuleEntity)
    {
        _actionRuleEntity = actionRuleEntity;
    }

    QString title() const
    {
        return _title;
    }
    void setTitle(const QString &title)
    {
        _title = title;
    }

    QStringList destinationPaths() const
    {
        return _destinationPaths;
    }
    void setDestinationPaths(const QStringList &destinationPaths)
    {
        _destinationPaths = destinationPaths;
    }

    QString appliesToPath() const
    {
        return _appliesToPath;
    }
    void setAppliesToPath(const QString &appliesToPath)
    {
        _appliesToPath = appliesToPath;
    }

    bool deepScanMode() const
    {
        return _deepScanMode;
    }
    void setDeepScanMode(bool deepScanMode)
    {
        _deepScanMode = deepScanMode;
    }

    QList<SubRule> subRules() const
    {
        return _subRules;
    }
    void setSubRules(const QList<SubRule> &subRules)
    {
        _subRules = subRules;
    }

private:
    RulesContext::FileType _typeFilter = RulesContext::File;
    RulesContext::RuleAction _actionRuleEntity;
    QString _title = "title";
    QStringList _destinationPaths;
    QString _appliesToPath = "Alle";
    bool _deepScanMode = false;
    QList<SubRule> _subRules;
};

#endif // RULES_H

class RuleBuilder
{
public:
    static Rule buildOrdinaryRule(QString title,
                                  QString appliesTo,
                                  QStringList destinations,
                                  RulesContext::RuleAction action,
                                  RulesContext::FileType type,
                                  QList<SubRule> subRules = QList<SubRule>(),
                                  bool deepScanMode = false)
    {
        Rule r;
        r.setTitle(title);
        r.setAppliesToPath(appliesTo);
        r.setDestinationPaths(destinations);
        r.setActionRuleEntity(action);
        r.setTypeFilter(type);
        r.setSubRules(subRules);
        r.setDeepScanMode(deepScanMode);

        return r;
    }

    static Rule &attachCriteria(SubRule sRule, Rule &r)
    {
        QList<SubRule> sRules = r.subRules();
        sRules.append(sRule);
        r.setSubRules(sRules);

        return r;
    }

    static SubRule buildSubRule(RulesContext::RuleCriteria criteria,
                                RulesContext::RuleCompareCriteria compareCriteria,
                                QStringList keyWords,
                                SizeLimit sizeLimit,
                                CustomDate date,
                                SizeLimits sizeInterval,
                                DateInterval dates,
                                bool matchWholeWords = false)
    {
        SubRule sRule;
        sRule.setCriteria(criteria);
        sRule.setCompareCriteria(compareCriteria);
        sRule.setKeyWords(keyWords);
        sRule.setSizeLimit(sizeLimit);
        sRule.setDate(date);
        sRule.setSizeInterval(sizeInterval);
        sRule.setDateIntervals(dates);

        sRule.setMatchWholeWords(matchWholeWords);

        return sRule;
    }
};


namespace RulesContext
{
    static QString ruleSizeLimitsToString(SubRule sRule)
    {
        QString minSize = QString::number(sRule.sizeInterval().first.first),
                maxSize = QString::number(sRule.sizeInterval().second.first);
        QString sizeUnitMin = sRule.sizeInterval().first.second,
                sizeUnitMax = sRule.sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }

    static QString ruleDateLimitsToString(SubRule sRule)
    {
        QString startDate = sRule.dateIntervals().first.date().toString("dd.MM.yyyy"),
                endDate = sRule.dateIntervals().second.date().toString("dd.MM.yyyy");
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

    static QString ruleKeyWordToString(SubRule sRule)
    {
        if(sRule.criteria() == RulesContext::fileSize &&
                sRule.compareCriteria() != RulesContext::interval)
            return QString::number(sRule.sizeLimit().first) + " " + sRule.sizeLimit().second;
        else if(sRule.criteria() == RulesContext::fileSize &&
                sRule.compareCriteria() == RulesContext::interval)
            return RulesContext::ruleSizeLimitsToString(sRule);
        else if((sRule.criteria() == RulesContext::fileCreatedMode || sRule.criteria() == RulesContext::fileModifiedMode) &&
                sRule.compareCriteria() != RulesContext::interval)
            return sRule.date().toString("dd.MM.yyyy");
        else if((sRule.criteria() == RulesContext::fileCreatedMode || sRule.criteria() == RulesContext::fileModifiedMode) &&
                sRule.compareCriteria() == RulesContext::interval)
            return RulesContext::ruleDateLimitsToString(sRule);
        else
            return RulesContext::mergeStringList(sRule.keyWords());
    }
}


