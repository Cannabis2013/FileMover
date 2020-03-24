#ifndef RULES_H
#define RULES_H

#include <QPair>
#include <QStringList>
#include "rulescontext.h"

class RuleCondition : public IDefaultRuleCondition
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

    int copyMode() const
    {
        return _copyMode;
    }
    void setCopyMode(const int &copymode)
    {
        _copyMode = copymode;
    }

    int compareCriteria() const
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const int &compareCriteria)
    {
        _compareCriteria = compareCriteria;
    }

    int criteria() const
    {
        return _criteria;
    }
    void setCriteria(const int &criteria)
    {
        _criteria = criteria;
    }

    QPair<quint64, QString> sizeLimit() const
    {
        return _sizeLimit;
    }
    void setSizeLimit(const QPair<quint64, QString> &sizeLimit)
    {
        _sizeLimit = sizeLimit;
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
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

class Rule : public IRule<IDefaultRuleCondition>
{
public:
    ~Rule()
    {
    }
    int typeFilter() const
    {
        return _typeFilter;
    }
    void setTypeFilter(const int &typeFilter)
    {
        _typeFilter = typeFilter;
    }

    int actionRuleEntity() const
    {
        return _actionRuleEntity;
    }
    void setActionRuleEntity(const int &actionRuleEntity)
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

    QList<const IDefaultRuleCondition*> conditions() const
    {
        return _subRules;
    }
    void setCriterias(const QList<const IDefaultRuleCondition*> &subRules)
    {
        _subRules = subRules;
    }

private:
    int _typeFilter = RulesContext::File;
    int _actionRuleEntity;
    QString _title = "title";
    QStringList _destinationPaths;
    QString _appliesToPath = "Alle";
    bool _deepScanMode = false;
    QList<const IDefaultRuleCondition*> _subRules;
};

#endif // RULES_H


