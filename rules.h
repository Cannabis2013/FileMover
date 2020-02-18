#ifndef RULES_H
#define RULES_H

#include "worker.h"
#include "ruledefinitions.h"
#include <QPair>
#include <QStringList>
#include "mydatetime.h"

typedef QPair<quint64,QString> SizeLimit;
typedef QPair<SizeLimit,SizeLimit> SizeLimits;
typedef QPair<myDateTime,myDateTime> DateInterval;

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

    myDateTime date() const
    {
        return _date;
    }
    void setDate(const myDateTime &date)
    {
        _date = date;
    }

    QPair<myDateTime, myDateTime> dateIntervals() const
    {
        return _dateIntervals;
    }
    void setDateIntervals(const QPair<myDateTime, myDateTime> &dateIntervals)
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

    RRT::CopyMode copyMode() const
    {
        return _copyMode;
    }
    void setCopyMode(const RRT::CopyMode &copymode)
    {
        _copyMode = copymode;
    }

    RRT::RuleCompareCriteria compareCriteria() const
    {
        return _compareCriteria;
    }
    void setCompareCriteria(const RRT::RuleCompareCriteria &compareCriteria)
    {
        _compareCriteria = compareCriteria;
    }

    RRT::RuleCriteria criteria() const
    {
        return _criteria;
    }
    void setCriteria(const RRT::RuleCriteria &criteria)
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
    RRT::CopyMode _copyMode = RRT::noMode;
    RRT::RuleCompareCriteria _compareCriteria = RRT::noCompareModeSet;
    RRT::RuleCriteria _criteria = RRT::nonConditionalMode;
    QPair<quint64,QString>_sizeLimit;
    SizeLimits _sizeInterval;
    myDateTime _date;
    QPair<myDateTime,myDateTime> _dateIntervals;
    bool _matchWholeWords = false;
    QStringList _keyWords;
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

class Rule
{
public:
    RRT::FileTypeEntity typeFilter() const
    {
        return _typeFilter;
    }
    void setTypeFilter(const RRT::FileTypeEntity &typeFilter)
    {
        _typeFilter = typeFilter;
    }

    RRT::RuleAction actionRuleEntity() const
    {
        return _actionRuleEntity;
    }
    void setActionRuleEntity(const RRT::RuleAction &actionRuleEntity)
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
    RRT::FileTypeEntity _typeFilter = RRT::File;
    RRT::RuleAction _actionRuleEntity;
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
                                  RRT::RuleAction action,
                                  RRT::FileTypeEntity type,
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

    static SubRule buildSubRule(RRT::RuleCriteria criteria,
                                RRT::RuleCompareCriteria compareCriteria,
                                QStringList keyWords,
                                SizeLimit sizeLimit,
                                myDateTime date,
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


