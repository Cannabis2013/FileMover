#ifndef RULES_H
#define RULES_H

#include <QPair>
#include <QStringList>
#include "rulescontext.h"

class RuleCondition : public IDefaultRuleCondition
{
public:
    ~RuleCondition(){}

    bool matchWholeWords() const override
    {
        return _matchWholeWords;
    }
    void setMatchWholeWords(bool matchWholeWords) override
    {
        _matchWholeWords = matchWholeWords;
    }

    SizeLimits sizeInterval() const override
    {
        return _sizeInterval;
    }
    void setSizeInterval(const SizeLimits &sizeInterval) override
    {
        _sizeInterval = sizeInterval;
    }

    QDateTime date() const override
    {
        return _date;
    }
    void setDate(const QDateTime &date) override
    {
        _date = date;
    }

    QPair<QDateTime, QDateTime> dateIntervals() const override
    {
        return _dateIntervals;
    }
    void setDateIntervals(const QPair<QDateTime, QDateTime> &dateIntervals) override
    {
        _dateIntervals = dateIntervals;
    }

    QStringList keyWords() const override
    {
        return _keyWords;
    }
    void setKeyWords(const QStringList &keyWords) override
    {
        _keyWords = keyWords;
    }

    int copyMode() const override
    {
        return _copyMode;
    }
    void setCopyMode(const int &copymode) override
    {
        _copyMode = copymode;
    }

    int compareCriteria() const override
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const int &compareCriteria) override
    {
        _compareCriteria = compareCriteria;
    }

    int criteria() const override
    {
        return _criteria;
    }
    void setCriteria(const int &criteria) override
    {
        _criteria = criteria;
    }

    QPair<quint64, QString> sizeLimit() const override
    {
        return _sizeLimit;
    }
    void setSizeLimit(const QPair<quint64, QString> &sizeLimit) override
    {
        _sizeLimit = sizeLimit;
    }

    bool operator==(const  IDefaultRuleCondition &other) const override
    {
        if(copyMode() != other.copyMode())
            return false;
        if(compareCriteria() != other.compareCriteria())
            return false;
        if(criteria() != other.criteria())
            return false;

        auto sizeUnit = sizeLimit().first;
        auto sizeDataStorageUnit = sizeLimit().second;

        auto otherSizeUnit = other.sizeLimit().first;
        auto otherSizeDataStorageUnit = other.sizeLimit().second;

        if(sizeUnit != otherSizeUnit || sizeDataStorageUnit != otherSizeDataStorageUnit)
            return false;

        auto lowerSize = sizeInterval().first;
        auto upperSize = sizeInterval().second;

        auto lowerSizeUnit = lowerSize.first;
        auto lowerSizeDataStorageUnit = lowerSize.second;

        auto upperSizeUnit = upperSize.first;
        auto upperSizeDataStorageUnit = upperSize.second;

        auto otherLowerSize = other.sizeInterval().first;
        auto otherUpperSize = other.sizeInterval().second;

        auto otherLowerSizeUnit = otherLowerSize.first;
        auto otherLowerSizeDataStorage = otherLowerSize.second;

        auto otherUpperSizeUnit = otherUpperSize.first;
        auto otherUpperSizeDataStorage = otherUpperSize.second;

        if(lowerSizeUnit != otherLowerSizeUnit)
            return false;
        if(lowerSizeDataStorageUnit != otherLowerSizeDataStorage)
            return false;
        if(upperSizeUnit != otherUpperSizeUnit)
            return false;
        if(upperSizeDataStorageUnit != otherUpperSizeDataStorage)
            return false;

        if(date() != other.date())
            return false;

        auto lowerDateLimit = dateIntervals().first;
        auto upperDateLimit = dateIntervals().second;

        auto otherLowerDateLimit = other.dateIntervals().first;
        auto otherUpperDateLimit = other.dateIntervals().second;

        if(lowerDateLimit != otherLowerDateLimit)
            return false;
        if(upperDateLimit != otherUpperDateLimit)
            return false;

        if(matchWholeWords() != other.matchWholeWords())
            return false;

        if(keyWords() != other.keyWords())
            return false;

        return true;
    }

    bool operator !=(const IDefaultRuleCondition &other) const override
    {
        return !this->operator==(other);
    }

private:
    int _copyMode = 0x01;
    int _compareCriteria = 0x01;
    int _criteria = 0x01;
    QPair<quint64,QString>_sizeLimit;
    SizeLimits _sizeInterval;
    QDateTime _date;
    QPair<QDateTime,QDateTime> _dateIntervals;
    bool _matchWholeWords = false;
    QStringList _keyWords;

    // IRuleCondition interface
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

class Rule : public IRule<IDefaultRuleCondition>
{
public:
    ~Rule()
    {
    }
    int typeFilter() const override
    {
        return _typeFilter;
    }
    void setTypeFilter(const int &typeFilter) override
    {
        _typeFilter = typeFilter;
    }

    int actionRuleEntity() const override
    {
        return _actionRuleEntity;
    }
    void setActionRuleEntity(const int &actionRuleEntity) override
    {
        _actionRuleEntity = actionRuleEntity;
    }

    QString title() const override
    {
        return _title;
    }
    void setTitle(const QString &title) override
    {
        _title = title;
    }

    QStringList destinationPaths() const override
    {
        return _destinationPaths;
    }
    void setDestinationPaths(const QStringList &destinationPaths) override
    {
        _destinationPaths = destinationPaths;
    }

    QString appliesToPath() const override
    {
        return _appliesToPath;
    }
    void setAppliesToPath(const QString &appliesToPath) override
    {
        _appliesToPath = appliesToPath;
    }

    bool deepScanMode() const override
    {
        return _deepScanMode;
    }
    void setDeepScanMode(bool deepScanMode) override
    {
        _deepScanMode = deepScanMode;
    }

    QList<const IDefaultRuleCondition*> conditions() const override
    {
        return _criterias;
    }
    void setCriterias(const QList<const IDefaultRuleCondition*> &subRules)
    {
        _criterias = subRules;
    }

    bool operator==(const IDefaultRule &other) const override
    {
        auto isCriteriasCovariant = [](const QList<const IDefaultRuleCondition*> &one, const QList<const IDefaultRuleCondition*> &two)->bool
        {
            if(one.count() != two.count())
                return false;


            for (int a = 0;a < one.count();a++) {
                auto firstItem = one.at(a);
                auto secondItem = two.at(a);

                if(firstItem != secondItem)
                    return false;
            }

            return true;
        };

        if(typeFilter() != other.typeFilter())
            return false;

        if(actionRuleEntity() != other.actionRuleEntity())
            return false;

        if(title() != other.title())
            return false;

        return isCriteriasCovariant(conditions(),other.conditions());
    }
    bool operator!=(const IDefaultRule &other) const override
    {
        return !this->operator==(other);
    }

private:
    int _typeFilter = RulesContext::File;
    int _actionRuleEntity;
    QString _title = "title";
    QStringList _destinationPaths;
    QString _appliesToPath = "Alle";
    bool _deepScanMode = false;
    QList<const IDefaultRuleCondition*> _criterias;
};

#endif // RULES_H


