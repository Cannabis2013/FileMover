#ifndef RULESCONTEXT_H
#define RULESCONTEXT_H

#include <QStringList>
#include "defaultRuleConfiguration.h"

namespace RulesContext
{
    enum RuleType {Action = 0x020,
                   Condition = 0x021,
                   CompareCriteria = 0x022,
                   All = 0x023};

    enum RuleAction{MoveAction = 0x024,
                    DeleteAction = 0x025,
                    CopyAction = 0x026,
                    NoAction = 0x01};

    enum RuleCriteria{NotDefined = 0x030,
                             FileBaseMode = 0x031,
                             FileNameMode = 0x032,
                             FileExtensionMode = 0x033,
                             FileSizeMode = 0x034,
                             FileParentMode = 0x035,
                             FileCreatedMode = 0x036, // FileDateMode
                             FileModifiedMode = 0x037, // FileDateMode
                             NonConditionalMode = 0x01};


    enum RuleCompareCriteria{Match = 0x040,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     DontMatch = 0x041,             // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     Contain = 0x042,              // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     DontContain = 0x043,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     LesserThan = 0x044,            // Filesize
                     LesserOrEqualThan = 0x045,     // Filesize
                     Equal = 0x046,                 // Filesize
                     GreaterOrEqualThan = 0x047,    // Filesize
                     GreaterThan = 0x048,           // Filesize
                     Interval = 0x049,              // Filesize/date interval
                     OlderThan = 0x050,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     ExactDate = 0x051,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     YoungerThan = 0x052,           // FileDateMode {fileCreatedMode,fileModifiedMode}
                     NoCompareModeSet = 0x01};

    enum FileType {Folder = 0x060, File = 0x061,Both = 0x062, Unresolved = 0x01};
    enum CopyMode{Move = 0x065,Copy = 0x066,NoMode = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
    static QString ruleSizeLimitsToString(const IDefaultConditionConfigurator *sRule)
    {
        QString minSize = QString::number(sRule->sizeInterval().first.first),
                maxSize = QString::number(sRule->sizeInterval().second.first);
        QString sizeUnitMin = sRule->sizeInterval().first.second,
                sizeUnitMax = sRule->sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }

    static QString ruleSizeLimitsToString(const IDefaultRuleCondition *sRule)
    {
        QString minSize = QString::number(sRule->sizeInterval().first.first),
                maxSize = QString::number(sRule->sizeInterval().second.first);
        QString sizeUnitMin = sRule->sizeInterval().first.second,
                sizeUnitMax = sRule->sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }

    static QString ruleDateLimitsToString(const IDefaultConditionConfigurator *sRule)
    {
        QString startDate = sRule->dates().first.date().toString("dd.MM.yyyy"),
                endDate = sRule->dates().second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }

    static QString ruleDateLimitsToString(const IDefaultRuleCondition *sRule)
    {
        QString startDate = sRule->dateIntervals().first.date().toString("dd.MM.yyyy"),
                endDate = sRule->dateIntervals().second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }

    static QString mergeStringList(const QStringList &strings)
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

    static QString ruleKeyWordToString(const IDefaultConditionConfigurator *sRule)
    {

        if(sRule->criteria() == RulesContext::FileSizeMode &&
                sRule->compareCriteria() != Interval)
            return QString::number(sRule->sizeLimit().first) + " " + sRule->sizeLimit().second;
        else if(sRule->criteria() == FileSizeMode &&
                sRule->compareCriteria() == Interval)
            return ruleSizeLimitsToString(sRule);
        else if((sRule->criteria() == FileCreatedMode || sRule->criteria() == FileModifiedMode) &&
                sRule->compareCriteria() != Interval)
            return sRule->date().toString("dd.MM.yyyy");
        else if((sRule->criteria() == FileCreatedMode || sRule->criteria() == FileModifiedMode) &&
                sRule->compareCriteria() == Interval)
            return ruleDateLimitsToString(sRule);
        else
            return mergeStringList(sRule->keywords());
    }

    static QString ruleKeyWordToString(const IDefaultRuleCondition *sRule)
    {

        if(sRule->criteria() == RulesContext::FileSizeMode &&
                sRule->compareCriteria() != Interval)
            return QString::number(sRule->sizeLimit().first) + " " + sRule->sizeLimit().second;
        else if(sRule->criteria() == FileSizeMode &&
                sRule->compareCriteria() == Interval)
            return ruleSizeLimitsToString(sRule);
        else if((sRule->criteria() == FileCreatedMode || sRule->criteria() == FileModifiedMode) &&
                sRule->compareCriteria() != Interval)
            return sRule->date().toString("dd.MM.yyyy");
        else if((sRule->criteria() == FileCreatedMode || sRule->criteria() == FileModifiedMode) &&
                sRule->compareCriteria() == Interval)
            return ruleDateLimitsToString(sRule);
        else
            return mergeStringList(sRule->keywords());
    }
}



#endif // RULESCONTEXT_H
