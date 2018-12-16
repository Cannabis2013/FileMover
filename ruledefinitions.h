#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qobject.h>

#include "worker.h"

/*
 *  Modes using 'keywords':
 *      containSuffix
 *      dontContainSuffix
 *      matchSuffix
 *      dontMatchSuffix
 *      containWords
 *      dontContainWords
 *      matchWords
 *      dontMatchWords
 *  Modes using 'widgets'
 *      dateCreatedMode
 *      dateModifiedMode
 *      sizeMode
 *      typeMode
 */

/*
 *  Fieldconditions
 *      notDefined
 *      filepatMode
 *      extensionMode
 *      sizeMode
 *      dateCreatedMode
 *      typeMode
 *      nonConditionalMode
 */

/*
 *  Comparemodes:
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

    const QList<QPair<QString,fileFieldCondition> > fields {
        QPair<QString,fileFieldCondition>("Filnavn",fileFieldCondition::filepathMode),
                QPair<QString,fileFieldCondition>("Filendelse",fileFieldCondition::extensionMode),
                QPair<QString,fileFieldCondition>("Størrelse",fileFieldCondition::sizeMode),
                QPair<QString,fileFieldCondition>("Dato oprettet",fileFieldCondition::dateCreatedMode),
                QPair<QString,fileFieldCondition>("Dato redigeret",fileFieldCondition::dateModifiedMode),
                QPair<QString,fileFieldCondition>("Type",fileFieldCondition::typeMode),
                QPair<QString,fileFieldCondition>("Ingen betingelser",fileFieldCondition::nonConditionalMode)};

    const QList<QPair<QString,compareMode> > compares {
        QPair<QString,compareMode>("Indeholder følgende",compareMode::contains),
                QPair<QString,compareMode>("Indeholder ikke følgende",compareMode::dontMatch),
                QPair<QString,compareMode>("Matcher følgende",compareMode::match),
                QPair<QString,compareMode>("Mathcer ikke følgende",compareMode::dontMatch),
                QPair<QString,compareMode>("Større end",compareMode::bigger),
                QPair<QString,compareMode>("Større eller lig med",compareMode::biggerOrEqual),
                QPair<QString,compareMode>("Lig med",compareMode::equal),
                QPair<QString,compareMode>("Mindre eller lig med",compareMode::lesserOrEqual),
                QPair<QString,compareMode>("Mindre Mindre end",compareMode::lesser),
                QPair<QString,compareMode>("Ældre end",compareMode::olderThan),
                QPair<QString,compareMode>("Præcis dato",compareMode::exactDate),
                QPair<QString,compareMode>("Yngre end",compareMode::youngerThan)};

    const QStringList fieldConditionalList {"Filnavn",
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


    const QStringList compareModeOperators(fileFieldCondition condition)
    {
        QStringList resultingList;
        if(condition == fileFieldCondition::filepathMode || condition == fileFieldCondition::extensionMode)
        {
            for(QPair<QString,compareMode> pair : compares) {
                if(pair.second == compareMode::contains ||
                        pair.second == compareMode::dontMatch ||
                        pair.second == compareMode::match ||
                        pair.second == compareMode::dontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileFieldCondition::sizeMode)
        {
            for(QPair<QString,compareMode> pair : compares)
            {
                if(pair.second == compareMode::bigger ||
                        compareMode::biggerOrEqual ||
                        compareMode::equal ||
                        compareMode::lesserOrEqual ||
                        compareMode::lesser)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileFieldCondition::dateCreatedMode ||
                condition == fileFieldCondition::dateModifiedMode)
        {
            for(QPair<QString,compareMode> pair : compares)
            {
                if(pair.second == compareMode::olderThan ||
                        compareMode::exactDate ||
                        compareMode::youngerThan)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileFieldCondition::typeMode)
        {

        }

        return resultingList;
    }

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

    QString fieldConditionToString(const fileFieldCondition mode)
    {
        for(QPair<QString,fileFieldCondition> pair : fields)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    fileFieldCondition fieldConditionFromString(const QString string)
    {
        for(QPair<QString,fileFieldCondition> pair : fields)
        {
            if(pair.first == string)
                return pair.second;
        }
    }

    QString compareToString(const compareMode mode)
    {
        for(QPair<QString,compareMode> pair : compares)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    compareMode compareFromString(const QString string)
    {
        for(QPair<QString,compareMode> pair : compares)
        {
            if(pair.first == string)
                return pair.second;
        }
        return compareMode::noCompareModeSet;
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
    static fileFieldCondition subConditionalFromString(const QString str)
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
