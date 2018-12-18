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
    enum typeProperty {actionProperty,conditionProperty, compareProperty, everyProperty};
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
    QStringList intervalConditionalList {"Enkel grænse","Interval grænse"};

    // List variables..
    static QStringList sizeUnits(){return QStringList{"b","kb","mb","gb"};}

    const QList<QPair<QString,fileActionRule> > actions {
        QPair<QString,fileActionRule>("Flytte indhold",fileActionRule::Move),
                QPair<QString,fileActionRule>("Slette indhold",fileActionRule::Delete),
                QPair<QString,fileActionRule>("Kopiere indhold",fileActionRule::Copy),
                QPair<QString,fileActionRule>("Gøre intet",fileActionRule::none)};

    const QList<QPair<QString,fileFieldCondition> > fields {
        QPair<QString,fileFieldCondition>("Filnavn",fileFieldCondition::filepathMode),
                QPair<QString,fileFieldCondition>("Filendelse",fileFieldCondition::extensionMode),
                QPair<QString,fileFieldCondition>("Størrelse",fileFieldCondition::sizeMode),
                QPair<QString,fileFieldCondition>("Dato oprettet",fileFieldCondition::dateCreatedMode),
                QPair<QString,fileFieldCondition>("Dato redigeret",fileFieldCondition::dateModifiedMode),
                QPair<QString,fileFieldCondition>("Type",fileFieldCondition::typeMode),
                QPair<QString,fileFieldCondition>("Ingen betingelser",fileFieldCondition::nonConditionalMode)};

    const QList<QPair<QString,compareMode> > compareOperators {
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

    // Retrieve list methods

    const QStringList propertyListToStrings(typeProperty property = typeProperty::everyProperty)
    {
        QStringList resultingList;
        if(property == typeProperty::actionProperty || property == typeProperty::everyProperty)
        {
            for ( QPair<QString,fileActionRule> pair : actions)
                resultingList << pair.first;
        }
        if(property == typeProperty::conditionProperty || property == typeProperty::everyProperty)
        {
            for ( QPair<QString,fileFieldCondition> pair : fields)
                resultingList << pair.first;
        }
        if(property == typeProperty::compareProperty || property == typeProperty::everyProperty)
        {
            for ( QPair<QString,compareMode> pair : compareOperators)
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList compareOperatorsToStringList(fileFieldCondition condition)
    {
        QStringList resultingList;
        if(condition == fileFieldCondition::filepathMode ||
                condition == fileFieldCondition::extensionMode)
        {
            for(QPair<QString,compareMode> pair : compareOperators) {
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
            for(QPair<QString,compareMode> pair : compareOperators)
            {
                if(pair.second == compareMode::bigger ||
                        pair.second ==compareMode::biggerOrEqual ||
                        pair.second ==compareMode::equal ||
                        pair.second ==compareMode::lesserOrEqual ||
                        pair.second == compareMode::lesser)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == fileFieldCondition::dateCreatedMode ||
                condition == fileFieldCondition::dateModifiedMode)
        {
            for(QPair<QString,compareMode> pair : compareOperators)
            {
                if(pair.second == compareMode::olderThan ||
                        pair.second == compareMode::exactDate ||
                        pair.second == compareMode::youngerThan)
                {
                    resultingList << pair.first;
                }
            }
        }

        return resultingList;
    }

    // From type1 to type2 methods

    fileActionRule actionFromString(const QString mode)
    {
        for(QPair<QString,fileActionRule> pair : actions)
        {
            if(pair.first == mode)
                return pair.second;
        }
        return fileActionRule::none;
    }
    QString actionToString(const fileActionRule mode)
    {
        for(QPair<QString,fileActionRule> pair : actions)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
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
        return fileFieldCondition::notDefined;
    }

    QString compareToString(const compareMode mode)
    {
        for(QPair<QString,compareMode> pair : compareOperators)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    compareMode compareFromString(const QString string)
    {
        for(QPair<QString,compareMode> pair : compareOperators)
        {
            if(pair.first == string)
                return pair.second;
        }
        return compareMode::noCompareModeSet;
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
        else if(tMode == bW::allEntries)
            return "Begge";
        else if(tMode == bW::noTypeSet)
            return "none";
        else
            return "error";
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
