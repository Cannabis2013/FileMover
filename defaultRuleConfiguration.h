#ifndef DEFAULTRULECONFIGURATION_H
#define DEFAULTRULECONFIGURATION_H

#include "iruleconfiguration.h"
#include "rules.h"

class RuleDefaultConfiguration : public IRuleConfiguration<IRuleCondition,RulesContext::FileType,RulesContext::RuleAction>
{
public:
    QString appliesTo() const
    {
        return _appliesTo;
    }
    void setAppliesTo(const QString &appliesTo)
    {
        _appliesTo = appliesTo;
    }

    QStringList destinations() const
    {
        return _destinations;
    }
    void setDestinations(const QStringList &destinations)
    {
        _destinations = destinations;
    }

    RulesContext::RuleAction action() const
    {
        return _action;
    }
    void setAction(const RulesContext::RuleAction &action)
    {
        _action = action;
    }

    RulesContext::FileType type() const
    {
        return _type;
    }
    void setType(const RulesContext::FileType &type)
    {
        _type = type;
    }

    QList<const IRuleCondition*> conditions() const
    {
        return _conditions;
    }
    void setConditions(const QList<const IRuleCondition *> &conditions)
    {
        _conditions = conditions;
    }

    bool deepScanMode() const
    {
        return _deepScanMode;
    }
    void setDeepScanMode(bool deepScanMode)
    {
        _deepScanMode = deepScanMode;
    }

    QString title() const
    {
        return _title;
    }
    void setTitle(const QString &title)
    {
        _title = title;
    }

private:
    QString _title,_appliesTo;
    QStringList _destinations;
    RulesContext::RuleAction _action;
    RulesContext::FileType _type;
    QList<const IRuleCondition*> _conditions;
    bool _deepScanMode;
};

class RuleConditionDefaultConfiguration :
        public IRuleConditionConfiguration<RulesContext::RuleCriteria,
                                            RulesContext::RuleCompareCriteria,
                                            SizeLimit,SizeLimits,
                                            QDateTime,DateInterval>
{
public:
    RulesContext::RuleCriteria criteria() const
    {
        return _criteria;
    }
    void setCriteria(const RulesContext::RuleCriteria &criteria)
    {
        _criteria = criteria;
    }

    RulesContext::RuleCompareCriteria compareCriteria() const
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const RulesContext::RuleCompareCriteria &compareCriteria)
    {
        _compareCriteria = compareCriteria;
    }

    QStringList keywords() const
    {
        return _keywords;
    }
    void setKeywords(const QStringList &value)
    {
        _keywords = value;
    }

    SizeLimit sizeLimit() const
    {
        return _sizeLimit;
    }
    void setSizeLimit(const SizeLimit &sizeLimit)
    {
        _sizeLimit = sizeLimit;
    }

    QDateTime date() const
    {
        return _date;
    }
    void setDate(const QDateTime &date)
    {
        _date = date;
    }

    SizeLimits sizeInterval() const
    {
        return _sizeInterval;
    }
    void setSizeInterval(const SizeLimits &sizeInterval)
    {
        _sizeInterval = sizeInterval;
    }

    DateInterval dates() const
    {
        return _dates;
    }
    void setDates(const DateInterval &dates)
    {
        _dates = dates;
    }

    bool matchWholeWords() const
    {
        return _matchWholeWords;
    }
    void setMatchWholeWords(bool matchWholeWords)
    {
        _matchWholeWords = matchWholeWords;
    }

private:
    RulesContext::RuleCriteria _criteria;
    RulesContext::RuleCompareCriteria _compareCriteria;
    QStringList _keywords;
    SizeLimit _sizeLimit;
    SizeLimits _sizeInterval;
    QDateTime _date;
    DateInterval _dates;
    bool _matchWholeWords = false;
};

#endif // DEFAULTRULECONFIGURATION_H
