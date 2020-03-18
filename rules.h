#ifndef RULES_H
#define RULES_H

#include <QPair>
#include <QStringList>
#include "iruleinterfaces.h"
#include "irulebuilder.h"

class RuleCondition : public IRuleCondition
{
public:
    ~RuleCondition(){}

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

    QDateTime date() const
    {
        return _date;
    }
    void setDate(const QDateTime &date)
    {
        _date = date;
    }

    QPair<QDateTime, QDateTime> dateIntervals() const
    {
        return _dateIntervals;
    }
    void setDateIntervals(const QPair<QDateTime, QDateTime> &dateIntervals)
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
    RulesContext::RuleCompareCriteria _compareCriteria = RulesContext::NoCompareModeSet;
    RulesContext::RuleCriteria _criteria = RulesContext::NonConditionalMode;
    QPair<quint64,QString>_sizeLimit;
    SizeLimits _sizeInterval;
    QDateTime _date;
    QPair<QDateTime,QDateTime> _dateIntervals;
    bool _matchWholeWords = false;
    QStringList _keyWords;
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

class Rule : public IRule<>
{
public:
    ~Rule()
    {
    }
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

    QList<const IRuleCondition*> conditions() const
    {
        return _subRules;
    }
    void setSubRules(const QList<const IRuleCondition*> &subRules)
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
    QList<const IRuleCondition*> _subRules;
};

#endif // RULES_H


