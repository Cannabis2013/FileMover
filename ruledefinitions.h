#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qlist.h>
#include <qtranslator.h>

#include "iruledefinitions.h"
#include "TypeDefinitions.h"

/*
 * Mappings:
 *      actionMappings<QString,enum>
 *      conditionMappings<QString,enum>
 *      compareMappings<QString,enum>
 *
 * RuleEntity types:
 *      fileActionEntity
 *      fileConditionEntity
 *      fileCompareEntity
 * fileActionEntity:
 *      Move
 *      Delete
 *      Copy
 *      None
 *
 *  fileConditionEntities
 *      notDefined
 *      filepathMode
 *      extensionMode
 *      parentFolderMode
 *      sizeMode
 *      dateCreatedMode
 *      nonConditionalMode
 *
 *  fileCompareEntity types categorized:
 *      Date:
 *          youngerThan
 *          exactDate
 *          olderThan
 *          interval
 *      Keyword:
 *          containSuffix
 *          dontContainSuffix
 *          matchSuffix
 *          dontMatchSuffix
 *          containWords
 *          dontContainWords
 *          matchWords
 *          dontMatchWords
 *      Size:
 *          lesser
 *          lesserOrequal
 *          equal
 *          biggerOrequal
 *          bigger
 *          sizeInterval
 *          interval
 *
 *  fileCompareEntities using QLineEdit widgets:
 *      containSuffix
 *      dontContainSuffix
 *      matchSuffix
 *      dontMatchSuffix
 *      containWords
 *      dontContainWords
 *      matchWords
 *      dontMatchWords
 *  fileConditionRuleEntities using custom widgets:
 *      dateCreatedMode
 *      dateModifiedMode
 *      sizeMode
 *      typeMode
 */

class RuleDefinitions : public IRuleDefinitions<RRT::RuleType,
        RRT::RuleAction,
        RRT::RuleCriteria,
        RRT::RuleCompareCriteria,
        RRT::FileTypeEntity>
{
public:
    // Enumerated variables..


    QString nonIntervalString = "Enkel grænse", intervalString = "Interval grænse";
    QStringList intervalConditionalList {"Enkel grænse","Interval grænse"};

    // List variables..
    QStringList sizeUnits(){return {"b","kb","mb","gb"};}


    /*
     * Mapping methods
     *      - actionMappings
     *      - conditionMappings
     *      - compareMappings
     *      - fileTypeFilterMappings
     */


    // Retrieve list methods

    QString buildDefaultStringValue()
    {
        return "No criterias";
    }

    const QStringList buildStringListFromEntity(RRT::RuleType property = RRT::all)
    {
        QStringList resultingList;
        if(property == RRT::action || property == RRT::all)
        {
            for ( QPair<QString,RRT::RuleAction> actionMapping : actionMappings())
                resultingList << actionMapping.first;
        }
        if(property == RRT::criteria || property == RRT::all)
        {
            for ( QPair<QString,RRT::RuleCriteria> pair : criteriaMappings())
                resultingList << pair.first;
        }
        if(property == RRT::compareCriteria || property == RRT::all)
        {
            for ( QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings())
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList fileCompareModesToStringList(RRT::RuleCriteria condition)
    {
        QStringList resultingList;
        if(condition == RRT::fileBaseMode ||
                condition == RRT::filepathMode ||
                condition == RRT::fileExtensionMode)
        {
            for(QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings()) {
                if(pair.second == RRT::contains ||
                        pair.second == RRT::dontMatch ||
                        pair.second == RRT::match ||
                        pair.second == RRT::dontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == RRT::fileSize)
        {
            for(QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == RRT::greaterThan ||
                        pair.second ==RRT::greaterOrEqualThan ||
                        pair.second ==RRT::equal ||
                        pair.second ==RRT::lesserOrEqualThan ||
                        pair.second == RRT::lesserThan)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == RRT::fileCreatedMode ||
                condition == RRT::fileModifiedMode)
        {
            for(QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == RRT::olderThan ||
                        pair.second == RRT::exactDate ||
                        pair.second == RRT::youngerThan)
                {
                    resultingList << pair.first;
                }
            }
        }

        return resultingList;
    }

    const QStringList allFileTypeEntitiesToStrings()
    {
        QStringList resultingList;

        for (QPair<QString,RRT::FileTypeEntity> pair : fileTypeFilterMappings())
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString fileActionEntityToString(const RRT::RuleAction mode)
    {
        for(QPair<QString,RRT::RuleAction> pair : actionMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    RRT::RuleAction fileActionEntityFromString(const QString mode)
    {
        for(QPair<QString,RRT::RuleAction> pair : actionMappings())
        {
            if(pair.first == mode)
                return pair.second;
        }
        return RRT::none;
    }

    QString buildStringFromCriteria(const RRT::RuleCriteria mode)
    {
        for(QPair<QString,RRT::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    RRT::RuleCriteria buildCriteriaFromString(const QString string)
    {
        for(QPair<QString,RRT::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return RRT::notDefined;
    }

    QString buildStringFromCompareCriteria(const RRT::RuleCompareCriteria mode)
    {
        for(QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    RRT::RuleCompareCriteria fileCompareEntityFromString(const QString string)
    {
        for(QPair<QString,RRT::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return RRT::noCompareModeSet;
    }

    QString fileTypeEntityToString(RRT::FileTypeEntity type)
    {
        for (QPair<QString,RRT::FileTypeEntity> pair : fileTypeFilterMappings())
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    RRT::FileTypeEntity fileTypeEntityFromString(const QString &string)
    {
        for (QPair<QString,RRT::FileTypeEntity> pair : fileTypeFilterMappings())
        {
            if(pair.first == string)
                return pair.second;
        }

        return RRT::unresolved;
    }

private:
    const QList<QPair<QString,RRT::RuleAction> > actionMappings() {
        return {
            QPair<QString,RRT::RuleAction>(("Move content"),RRT::RuleAction::Move),
            QPair<QString,RRT::RuleAction>("Delete content",RRT::Delete),
            QPair<QString,RRT::RuleAction>("Copy content",RRT::Copy),
            QPair<QString,RRT::RuleAction>("Do nothing",RRT::none)};
        }

    const QList<QPair<QString,RRT::RuleCriteria> > criteriaMappings() {
        return {
            QPair<QString,RRT::RuleCriteria>("Base name",RRT::fileBaseMode),
            QPair<QString,RRT::RuleCriteria>("File name",RRT::filepathMode),
            QPair<QString,RRT::RuleCriteria>("File suffix",RRT::fileExtensionMode),
            QPair<QString,RRT::RuleCriteria>("File size",RRT::fileSize),
            QPair<QString,RRT::RuleCriteria>("In folder",RRT::fileParentMode),
            QPair<QString,RRT::RuleCriteria>("Date created",RRT::fileCreatedMode),
            QPair<QString,RRT::RuleCriteria>("Date edited",RRT::fileModifiedMode),
            QPair<QString,RRT::RuleCriteria>("No conditions",RRT::nonConditionalMode)};
        }

    const QList<QPair<QString,RRT::RuleCompareCriteria> > compareCriteriaMappings() {

        return {
            QPair<QString,RRT::RuleCompareCriteria>("Contains",RRT::contains),
            QPair<QString,RRT::RuleCompareCriteria>("Does not contain",RRT::dontMatch),
            QPair<QString,RRT::RuleCompareCriteria>("Matching",RRT::match),
            QPair<QString,RRT::RuleCompareCriteria>("Does not match",RRT::dontMatch),
            QPair<QString,RRT::RuleCompareCriteria>("Greater than",RRT::greaterThan),
            QPair<QString,RRT::RuleCompareCriteria>("Greater or equal than",RRT::greaterOrEqualThan),
            QPair<QString,RRT::RuleCompareCriteria>("Equal",RRT::equal),
            QPair<QString,RRT::RuleCompareCriteria>("Less or equal than",RRT::lesserOrEqualThan),
            QPair<QString,RRT::RuleCompareCriteria>("Equal than",RRT::lesserThan),
            QPair<QString,RRT::RuleCompareCriteria>("Older than",RRT::olderThan),
            QPair<QString,RRT::RuleCompareCriteria>("Extact date",RRT::exactDate),
            QPair<QString,RRT::RuleCompareCriteria>("Younger than",RRT::youngerThan)};
        }

    const QList<QPair<QString,RRT::FileTypeEntity> > fileTypeFilterMappings()
    {
        return {
            QPair<QString,RRT::FileTypeEntity>("Files", RRT::File),
            QPair<QString,RRT::FileTypeEntity>("Folders", RRT::Folder),
            QPair<QString,RRT::FileTypeEntity>("Undetermined", RRT::unresolved)
        };
    };
};

typedef IRuleDefinitions<RRT::RuleType,RRT::RuleAction,RRT::RuleCriteria,RRT::RuleCompareCriteria,RRT::FileTypeEntity> IDefinitions;

#endif // RULEDEFINITIONS_H
