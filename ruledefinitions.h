#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qobject.h>
#include "worker.h"

/*
 * Modes using 'keywords':
 *      containSuffix
 *      dontContainSuffix
 *      matchSuffix
 *      dontMatchSuffix
 *      containWords
 *      dontContainWords
 *      matchWords
 *      dontMatchWords
 * Modes using 'custom widgets'
 *      dateCreatedMode
 *      dateModifiedMode
 *      sizeMode
 *      typeMode
*/

/*
 * Comparemodes:
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
struct ruleDefinitions
{
    // Enumerated variables..
    enum copyMode{move,copy,noMode};
    enum fileActionRule{Move,Delete,Copy,none};
    enum fileFieldCondition{notDefined,
                      filepathMode,
                      extensionMode,
                      sizeMode,
                      dateCreatedMode,
                      dateModifiedMode,
                      typeMode,
                      nonConditionalMode};

    enum compareMode{match,
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

    QString nonIntervalString = "Enkel grænse", intervalString = "Interval grænse";


    // List variables..
    static QStringList sizeUnits(){return QStringList{"b","kb","mb","gb"};}

    const QStringList actionList {"flytte filer",
                                   "slette filer",
                                   "kopiere filer"};

    const QStringList conditionalList {"Filnavn",
                                       "Filendelse",
                                        "Størrelse",
                                        "Dato oprettet",
                                        "Dato redigeret",
                                        "Type",
                                        "Ingen betingelser"};
    const QStringList fileNameCompareModeList {"Indeholder følgende",
                                              "Indeholder ikke følgende",
                                              "Matcher følgende",
                                              "Matcher ikke følgende"};

    // To conditionWidget related..
    const QStringList sizeOperators {"Større end","Større eller lig med","Lig med","Mindre end","Mindre eller lig med"};
    const QStringList dateOperators {"Ældre end","Præcis dato","Yngre end"};

    // Methods..
    static fileActionRule actionFromString(const QString str)
    {
        if(str == "Move" || str == "flytte filer")
            return fileActionRule::Move;
        if(str == "Delete" || str == "slette filer")
            return fileActionRule::Delete;
        if(str == "Copy" || str == "kopiere filer")
            return fileActionRule::Copy;

        return fileActionRule::none;
    }
    static QString actionToString(const fileActionRule act)
    {
        if(act == fileActionRule::Move)
            return "flytte filer";
        else if(act == fileActionRule::Delete)
            return "slette filer";
        else if(act == fileActionRule::Copy)
            return "kopiere filer";
        else
            return QString();
    }

    const QStringList intervalConditionalList()
    {
        return {nonIntervalString,intervalString};
    }

    static QString fieldConditionalToString(const fileFieldCondition mode)
    {
        if(mode == fileFieldCondition::filepathMode)
            return "Filnavn";
        else if(mode == fileFieldCondition::extensionMode)
            return "Filendelse";
        else if(mode == fileFieldCondition::sizeMode)
            return "Størrelse";
        else if(mode == fileFieldCondition::dateCreatedMode)
            return "Dato oprettet";
        else if(mode == fileFieldCondition::dateModifiedMode)
            return "Dato redigeret";
        else if(mode == fileFieldCondition::typeMode)
            return "Type";
        else if(mode == fileFieldCondition::nonConditionalMode)
            return "Ingen betingelser";
        else
            return "Ikke defineret";
    }
    static fileFieldCondition conditionalFromString(const QString str)
    {
        if(str == "Filnavn")
        {
            return fileFieldCondition::filepathMode;
        }
        else if(str == "Filendelse")
        {
            return fileFieldCondition::extensionMode;
        }
        else if(str == "Størrelse")
        {
            return fileFieldCondition::sizeMode;
        }
        else if(str == "Dato oprettet")
        {
            return fileFieldCondition::dateCreatedMode;
        }
        else if(str == "Dato redigeret")
        {
            return fileFieldCondition::dateModifiedMode;
        }
        else if(str == "Type")
        {
            return fileFieldCondition::typeMode;
        }
        else if(str == "Ingen betingelser")
        {
            return fileFieldCondition::nonConditionalMode;
        }
        else
            return fileFieldCondition::notDefined;
    }

    static QStringList typeList()
    {
        return QStringList {"Mapper",
            "Filer"};
    }
    QString typeFromIndex(int index)
    {
        return typeList().at(index);
    }
    static QString typeFromEnum(bW::iteratorMode tMode)
    {
        if(tMode == bW::folderOnly)
            return "Mapper";
        else if(tMode == bW::filesOnly)
            return "Filer";
        else if(bW::allEntries)
            return "Begge";
        else if(bW::noTypeSet)
            return "none";
        else
            return "error";
    }

    static compareMode compareModeFromString(QString str)
    {
        if(str == "Indeholder følgende" || str == "Contains")
            return compareMode::contains;
        if(str == "Indeholder ikke følgende" || str == "Dont contains")
            return compareMode::dontContain;
        if(str == "Matcher følgende" || str == "Match")
            return compareMode::match;
        if(str == "Matcher ikke følgende" || str == "Dont match")
            return compareMode::dontMatch;
        if(str == "Større end" || str == "bigger")
            return compareMode::bigger;
        if(str == "Større eller lig med" || str == "BiggerOrEqual")
            return compareMode::biggerOrEqual;
        if(str == "Lig med" || str == "Equal")
            return compareMode::equal;
        if(str == "Mindre eller lig med" || str == "LesserOrEqual")
            return compareMode::lesserOrEqual;
        if(str == "Mindre end" || str == "Lesser")
            return compareMode::lesser;
        if(str == "Ældre end")
            return compareMode::olderThan;
        if(str == "Præcis dato")
            return compareMode::exactDate;
        if(str == "Yngre end")
            return compareMode::youngerThan;

        return compareMode::noCompareModeSet;
    }
    static QString compareModeToString(compareMode compMode)
    {
        if(compMode == compareMode::contains)
            return "Indeholder følgende:";
        else if(compMode == compareMode::dontContain)
            return "Indeholder ikke følgende";
        else if(compMode == compareMode::match)
            return "Matcher følgende";
        else if(compMode == compareMode::dontMatch)
            return "Matcher ikke følgende";
        else if(compMode == compareMode::lesser)
            return "Mindre end";
        else if(compMode == compareMode::lesserOrEqual)
            return "Mindre eller lig med";
        else if(compMode == compareMode::equal)
            return "Lig med";
        else if(compMode == compareMode::biggerOrEqual)
            return "Større eller lig med";
        else if(compMode == compareMode::bigger)
            return "Større end";
        else if(compMode == compareMode::interval)
            return "I intervallet";
        else if(compMode == compareMode::olderThan)
            return "Ældre end";
        else if(compMode == compareMode::exactDate)
            return "Præcis dato";
        else if(compMode == compareMode::youngerThan)
            return "Yngre end";
        else if(compMode == compareMode::noDateSet)
            return "Ingen dato fastsat";
        else if(compMode == noCompareModeSet)
            return "Ingen betingelse";
        else
            return "Noget gik galt!";
    }
    static compareMode dateModeFromString(const QString str)
    {
        if(str == "Ældre end")
            return compareMode::olderThan;
        else if(str == "Præcis dato")
            return compareMode::exactDate;
        else if(str == "Yngre end")
            return compareMode::youngerThan;

        return compareMode::noDateSet;
    }

    static Worker::iteratorMode typeModeFromString(const QString str)
    {
        if(str == "Mapper")
            return Worker::folderOnly;
        else if(str == "Filer")
            return Worker::filesOnly;
        else
            return Worker::allEntries;
    }
};
typedef ruleDefinitions rD;



#endif // RULEDEFINITIONS_H
