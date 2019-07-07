#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qobject.h>
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
 *      parentFolderMode
 *      sizeMode
 *      dateCreatedMode
 *      nonConditionalMode
 */

/*
 *  fileCompareRuleEntity:
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
    enum ruleEntityType {actionProperty,conditionProperty, compareProperty, everyProperty};
    enum fileActionRuleEntity{Move,Delete,Copy,none};
    enum fileConditionRuleEntity{notDefined,
                                 filepathMode,
                                 extensionMode,
                                 sizeMode,
                                 parentFolderMode,
                                 dateCreatedMode,
                                 dateModifiedMode,
                                 nonConditionalMode};

    enum fileCompareRuleEntity{match,
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

    enum fileTypeRuleEntity {Folder, File, unresolved};

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

    const QList<QPair<QString,fileActionRuleEntity> > actionMappings {
        QPair<QString,fileActionRuleEntity>("Flytte indhold",fileActionRuleEntity::Move),
                QPair<QString,fileActionRuleEntity>("Slette indhold",fileActionRuleEntity::Delete),
                QPair<QString,fileActionRuleEntity>("Kopiere indhold",fileActionRuleEntity::Copy),
                QPair<QString,fileActionRuleEntity>("Gøre intet",fileActionRuleEntity::none)};

    const QList<QPair<QString,fileConditionRuleEntity> > conditionMappings {
        QPair<QString,fileConditionRuleEntity>("Filnavn",fileConditionRuleEntity::filepathMode),
                QPair<QString,fileConditionRuleEntity>("Filendelse",fileConditionRuleEntity::extensionMode),
                QPair<QString,fileConditionRuleEntity>("Størrelse",fileConditionRuleEntity::sizeMode),
                QPair<QString,fileConditionRuleEntity>("I mappen",fileConditionRuleEntity::parentFolderMode),
                QPair<QString,fileConditionRuleEntity>("Dato oprettet",fileConditionRuleEntity::dateCreatedMode),
                QPair<QString,fileConditionRuleEntity>("Dato redigeret",fileConditionRuleEntity::dateModifiedMode),
                QPair<QString,fileConditionRuleEntity>("Ingen betingelser",fileConditionRuleEntity::nonConditionalMode)};

    const QList<QPair<QString,fileCompareRuleEntity> > compareMappings {
        QPair<QString,fileCompareRuleEntity>("Indeholder følgende",fileCompareRuleEntity::contains),
                QPair<QString,fileCompareRuleEntity>("Indeholder ikke følgende",fileCompareRuleEntity::dontMatch),
                QPair<QString,fileCompareRuleEntity>("Matcher følgende",fileCompareRuleEntity::match),
                QPair<QString,fileCompareRuleEntity>("Mathcer ikke følgende",fileCompareRuleEntity::dontMatch),
                QPair<QString,fileCompareRuleEntity>("Større end",fileCompareRuleEntity::bigger),
                QPair<QString,fileCompareRuleEntity>("Større eller lig med",fileCompareRuleEntity::biggerOrEqual),
                QPair<QString,fileCompareRuleEntity>("Lig med",fileCompareRuleEntity::equal),
                QPair<QString,fileCompareRuleEntity>("Mindre eller lig med",fileCompareRuleEntity::lesserOrEqual),
                QPair<QString,fileCompareRuleEntity>("Mindre end",fileCompareRuleEntity::lesser),
                QPair<QString,fileCompareRuleEntity>("Ældre end",fileCompareRuleEntity::olderThan),
                QPair<QString,fileCompareRuleEntity>("Præcis dato",fileCompareRuleEntity::exactDate),
                QPair<QString,fileCompareRuleEntity>("Yngre end",fileCompareRuleEntity::youngerThan)};

    const QList<QPair<QString,fileTypeRuleEntity> > fileTypeFilterMappings
    {
        QPair<QString,fileTypeRuleEntity>("Filer", fileTypeRuleEntity::File),
                QPair<QString,fileTypeRuleEntity>("Mapper", fileTypeRuleEntity::Folder),
                QPair<QString,fileTypeRuleEntity>("Uafklaret", fileTypeRuleEntity::unresolved)
    };

    // Retrieve list methods

    const QStringList allRuleStringEntities(ruleEntityType property = ruleEntityType::everyProperty)
    {
        QStringList resultingList;
        if(property == ruleEntityType::actionProperty || property == ruleEntityType::everyProperty)
        {
            for ( QPair<QString,fileActionRuleEntity> actionMapping : actionMappings)
                resultingList << actionMapping.first;
        }
        if(property == ruleEntityType::conditionProperty || property == ruleEntityType::everyProperty)
        {
            for ( QPair<QString,fileConditionRuleEntity> pair : conditionMappings)
                resultingList << pair.first;
        }
        if(property == ruleEntityType::compareProperty || property == ruleEntityType::everyProperty)
        {
            for ( QPair<QString,fileCompareRuleEntity> pair : compareMappings)
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList allCompareStringEntities(fileConditionRuleEntity condition)
    {
        QStringList resultingList;
        if(condition == fileConditionRuleEntity::filepathMode ||
                condition == fileConditionRuleEntity::extensionMode)
        {
            for(QPair<QString,fileCompareRuleEntity> pair : compareMappings) {
                if(pair.second == fileCompareRuleEntity::contains ||
                        pair.second == fileCompareRuleEntity::dontMatch ||
                        pair.second == fileCompareRuleEntity::match ||
                        pair.second == fileCompareRuleEntity::dontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileConditionRuleEntity::sizeMode)
        {
            for(QPair<QString,fileCompareRuleEntity> pair : compareMappings)
            {
                if(pair.second == fileCompareRuleEntity::bigger ||
                        pair.second ==fileCompareRuleEntity::biggerOrEqual ||
                        pair.second ==fileCompareRuleEntity::equal ||
                        pair.second ==fileCompareRuleEntity::lesserOrEqual ||
                        pair.second == fileCompareRuleEntity::lesser)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileConditionRuleEntity::dateCreatedMode ||
                condition == fileConditionRuleEntity::dateModifiedMode)
        {
            for(QPair<QString,fileCompareRuleEntity> pair : compareMappings)
            {
                if(pair.second == fileCompareRuleEntity::olderThan ||
                        pair.second == fileCompareRuleEntity::exactDate ||
                        pair.second == fileCompareRuleEntity::youngerThan)
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

        for (QPair<QString,fileTypeRuleEntity> pair : fileTypeFilterMappings)
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString actionToString(const fileActionRuleEntity mode)
    {
        for(QPair<QString,fileActionRuleEntity> pair : actionMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileActionRuleEntity actionFromString(const QString mode)
    {
        for(QPair<QString,fileActionRuleEntity> pair : actionMappings)
        {
            if(pair.first == mode)
                return pair.second;
        }
        return fileActionRuleEntity::none;
    }

    QString fieldConditionToString(const fileConditionRuleEntity mode)
    {
        for(QPair<QString,fileConditionRuleEntity> pair : conditionMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileConditionRuleEntity fieldConditionFromString(const QString string)
    {
        for(QPair<QString,fileConditionRuleEntity> pair : conditionMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return fileConditionRuleEntity::notDefined;
    }

    QString compareToString(const fileCompareRuleEntity mode)
    {
        for(QPair<QString,fileCompareRuleEntity> pair : compareMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileCompareRuleEntity compareFromString(const QString string)
    {
        for(QPair<QString,fileCompareRuleEntity> pair : compareMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return fileCompareRuleEntity::noCompareModeSet;
    }

    QString fileTypeToString(fileTypeRuleEntity type)
    {
        for (QPair<QString,fileTypeRuleEntity> pair : fileTypeFilterMappings)
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    fileTypeRuleEntity fileTypeFromString(QString string)
    {
        for (QPair<QString,fileTypeRuleEntity> pair : fileTypeFilterMappings)
        {
            if(pair.first == string)
                return pair.second;
        }

        return fileTypeRuleEntity::unresolved;
    }
};
typedef ruleDefinitions rD;

#endif // RULEDEFINITIONS_H
