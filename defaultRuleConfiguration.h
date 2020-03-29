#ifndef DEFAULTRULECONFIGURATION_H
#define DEFAULTRULECONFIGURATION_H

#include "iruleconfiguration.h"
#include "idefaultrulescollection.h"

class DefaultRuleConfiguration : public IRuleConfiguration<IDefaultRuleCondition>
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

    int action() const
    {
        return _action;
    }
    void setAction(const int &action)
    {
        _action = action;
    }

    int type() const
    {
        return _type;
    }
    void setType(const int &type)
    {
        _type = type;
    }

    QList<const IDefaultRuleCondition*> conditions() const
    {
        return _conditions;
    }
    void setConditions(const QList<const IDefaultRuleCondition *> &conditions)
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
    int _action;
    int _type;
    QList<const IDefaultRuleCondition*> _conditions;
    bool _deepScanMode;
};

class DefaultCriteriaConfiguration :
        public IRuleConditionConfiguration<SizeLimit,SizeLimits,
                                            QDateTime>
{
public:
    int criteria() const
    {
        return _criteria;
    }
    void setCriteria(const int &criteria)
    {
        _criteria = criteria;
    }

    int compareCriteria() const
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const int &compareCriteria)
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
    void setSizeInterval(const int &minSize, const QString &minDSU, const int &maxSize, const QString &maxDSU)
    {
        auto min = SizeLimit(minSize,minDSU);
        auto max = SizeLimit(maxSize,maxDSU);

        auto sizeInterval = SizeLimits(min,max);

        _sizeInterval = sizeInterval;
    }

    const DateInterval dates() const
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
    int _criteria;
    int _compareCriteria;
    QStringList _keywords;
    SizeLimit _sizeLimit;
    SizeLimits _sizeInterval;
    QDateTime _date;
    DateInterval _dates;
    bool _matchWholeWords = false;
};

#endif // DEFAULTRULECONFIGURATION_H
