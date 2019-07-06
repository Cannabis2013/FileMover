#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qobject.h>

#include "worker.h"

/*
 * Mappings:
 *      actionMappings<QString,enum>
 *      conditionMappings<QString,enum>
 *      compareMappings<QString,enum>
 */

/*
 * RuleEntity:
 *      fileActionRuleEntity
 *      fileConditionRuleEntity
 *      fileCompareRuleEntity
 */

/*
 * fileActionRuleEntity:
 *      Move
 *      Delete
 *      Copy
 *      None
 */

/*
 *  fileConditionRuleEntities
 *      notDefined
 *      filepathMode
 *      extensionMode
 *      sizeMode
 *      dateCreatedMode
 *      nonConditionalMode
 */

/*
 *  fileCompareRuleEntity:
 *      Size:
 *          lesser
 *          lesserOrequal
 *          equal
 *          biggerOrequal
 *          bigger
 *          sizeInterval
 *          interval
 *      Date:
 *          youngerThan
 *          exactDate
 *          olderThan
 *          interval
 */

/*
 *  fileCompareRuleEntities using 'keywords':
 *      containSuffix
 *      dontContainSuffix
 *      matchSuffix
 *      dontMatchSuffix
 *      containWords
 *      dontContainWords
 *      matchWords
 *      dontMatchWords
 *  fileConditionRuleEntities using 'widgets'
 *      dateCreatedMode
 *      dateModifiedMode
 *      sizeMode
 *      typeMode
 */
struct ruleDefinitions
{
    // Enumerated variables..
    enum conditionEntityType {actionProperty,conditionProperty, compareProperty, everyProperty};
    enum fileAction{Move,Delete,Copy,none};
    enum fileCondition{notDefined,
                      filepathMode,
                      extensionMode,
                      sizeMode,
                      dateCreatedMode,
                      dateModifiedMode,
                      nonConditionalMode};

    enum fileComparison{match,
                     dontMatch,
                     contains,
                     dontContain,
                     lesser,
                     lesserOrEqual,
                     equal,
                     biggerOrEqual,
                     bigger,
                     interval,
                     olderThan,
                     exactDate,
                     youngerThan,
                     noDateSet,
                     noCompareModeSet};

    enum fileTypeFilter {Folder, File, Both,unresolved};
    enum copyMode{move,copy,noMode};

    QString nonIntervalString = "Enkel grænse", intervalString = "Interval grænse";
    QStringList intervalConditionalList {"Enkel grænse","Interval grænse"};

    // List variables..
    static QStringList sizeUnits(){return QStringList{"b","kb","mb","gb"};}


    /*
     * Mappings
     *      - actionMappings
     *      - conditionMappings
     *      - compareMappings
     *      - fileTypeFilterMappings
     */

    const QList<QPair<QString,fileAction> > actionMappings {
        QPair<QString,fileAction>("Flytte indhold",fileAction::Move),
                QPair<QString,fileAction>("Slette indhold",fileAction::Delete),
                QPair<QString,fileAction>("Kopiere indhold",fileAction::Copy),
                QPair<QString,fileAction>("Gøre intet",fileAction::none)};

    const QList<QPair<QString,fileCondition> > conditionMappings {
        QPair<QString,fileCondition>("Filnavn",fileCondition::filepathMode),
                QPair<QString,fileCondition>("Filendelse",fileCondition::extensionMode),
                QPair<QString,fileCondition>("Størrelse",fileCondition::sizeMode),
                QPair<QString,fileCondition>("Dato oprettet",fileCondition::dateCreatedMode),
                QPair<QString,fileCondition>("Dato redigeret",fileCondition::dateModifiedMode),
                QPair<QString,fileCondition>("Ingen betingelser",fileCondition::nonConditionalMode)};

    const QList<QPair<QString,fileComparison> > compareMappings {
        QPair<QString,fileComparison>("Indeholder følgende",fileComparison::contains),
                QPair<QString,fileComparison>("Indeholder ikke følgende",fileComparison::dontMatch),
                QPair<QString,fileComparison>("Matcher følgende",fileComparison::match),
                QPair<QString,fileComparison>("Mathcer ikke følgende",fileComparison::dontMatch),
                QPair<QString,fileComparison>("Større end",fileComparison::bigger),
                QPair<QString,fileComparison>("Større eller lig med",fileComparison::biggerOrEqual),
                QPair<QString,fileComparison>("Lig med",fileComparison::equal),
                QPair<QString,fileComparison>("Mindre eller lig med",fileComparison::lesserOrEqual),
                QPair<QString,fileComparison>("Mindre end",fileComparison::lesser),
                QPair<QString,fileComparison>("Ældre end",fileComparison::olderThan),
                QPair<QString,fileComparison>("Præcis dato",fileComparison::exactDate),
                QPair<QString,fileComparison>("Yngre end",fileComparison::youngerThan)};

    const QList<QPair<QString,fileTypeFilter> > fileTypeFilterMappings
    {
        QPair<QString,fileTypeFilter>("Filer", fileTypeFilter::File),
                QPair<QString,fileTypeFilter>("Mapper", fileTypeFilter::Folder),
                QPair<QString,fileTypeFilter>("Begge", fileTypeFilter::Both),
                QPair<QString,fileTypeFilter>("Uafklaret", fileTypeFilter::unresolved)
    };

    // Retrieve list methods

    const QStringList propertyListToStrings(conditionEntityType property = conditionEntityType::everyProperty)
    {
        QStringList resultingList;
        if(property == conditionEntityType::actionProperty || property == conditionEntityType::everyProperty)
        {
            for ( QPair<QString,fileAction> actionMapping : actionMappings)
                resultingList << actionMapping.first;
        }
        if(property == conditionEntityType::conditionProperty || property == conditionEntityType::everyProperty)
        {
            for ( QPair<QString,fileCondition> pair : conditionMappings)
                resultingList << pair.first;
        }
        if(property == conditionEntityType::compareProperty || property == conditionEntityType::everyProperty)
        {
            for ( QPair<QString,fileComparison> pair : compareMappings)
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList compareOperatorsToStringList(fileCondition condition)
    {
        QStringList resultingList;
        if(condition == fileCondition::filepathMode ||
                condition == fileCondition::extensionMode)
        {
            for(QPair<QString,fileComparison> pair : compareMappings) {
                if(pair.second == fileComparison::contains ||
                        pair.second == fileComparison::dontMatch ||
                        pair.second == fileComparison::match ||
                        pair.second == fileComparison::dontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileCondition::sizeMode)
        {
            for(QPair<QString,fileComparison> pair : compareMappings)
            {
                if(pair.second == fileComparison::bigger ||
                        pair.second ==fileComparison::biggerOrEqual ||
                        pair.second ==fileComparison::equal ||
                        pair.second ==fileComparison::lesserOrEqual ||
                        pair.second == fileComparison::lesser)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileCondition::dateCreatedMode ||
                condition == fileCondition::dateModifiedMode)
        {
            for(QPair<QString,fileComparison> pair : compareMappings)
            {
                if(pair.second == fileComparison::olderThan ||
                        pair.second == fileComparison::exactDate ||
                        pair.second == fileComparison::youngerThan)
                {
                    resultingList << pair.first;
                }
            }
        }

        return resultingList;
    }

    const QStringList fullTypeFilterPropertyList()
    {
        QStringList resultingList;

        for (QPair<QString,fileTypeFilter> pair : fileTypeFilterMappings)
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString actionToString(const fileAction mode)
    {
        for(QPair<QString,fileAction> pair : actionMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileAction actionFromString(const QString mode)
    {
        for(QPair<QString,fileAction> pair : actionMappings)
        {
            if(pair.first == mode)
                return pair.second;
        }
        return fileAction::none;
    }

    QString fieldConditionToString(const fileCondition mode)
    {
        for(QPair<QString,fileCondition> pair : conditionMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileCondition fieldConditionFromString(const QString string)
    {
        for(QPair<QString,fileCondition> pair : conditionMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return fileCondition::notDefined;
    }

    QString compareToString(const fileComparison mode)
    {
        for(QPair<QString,fileComparison> pair : compareMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileComparison compareFromString(const QString string)
    {
        for(QPair<QString,fileComparison> pair : compareMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return fileComparison::noCompareModeSet;
    }

    QString fileTypeToString(fileTypeFilter type)
    {
        for (QPair<QString,fileTypeFilter> pair : fileTypeFilterMappings)
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    fileTypeFilter fileTypeFromString(QString string)
    {
        for (QPair<QString,fileTypeFilter> pair : fileTypeFilterMappings)
        {
            if(pair.first == string)
                return pair.second;
        }

        return fileTypeFilter::unresolved;
    }
};
typedef ruleDefinitions rD;

#endif // RULEDEFINITIONS_H
