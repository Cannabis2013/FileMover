#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qlist.h>
#include <qtranslator.h>


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

struct ruleDefinitions
{
    // Enumerated variables..
    enum ruleType {action,
                   criteria,
                   compareCriteria,
                   all};

    enum ruleAction{Move,
                    Delete,
                    Copy,
                    none};

    enum ruleCriteria{notDefined,
                             fileBaseMode,
                             filepathMode,
                             fileExtensionMode,
                             fileSize,
                             fileParentMode,
                             fileCreatedMode, // FileDateMode
                             fileModifiedMode, // FileDateMode
                             nonConditionalMode};


    enum ruleCompareCriteria{match,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontMatch,             // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     contains,              // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontContain,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     lesserThan,            // Filesize
                     lesserOrEqualThan,     // Filesize
                     equal,                 // Filesize
                     greaterOrEqualThan,    // Filesize
                     greaterThan,           // Filesize
                     interval,              // Filesize/date interval
                     olderThan,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     exactDate,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     youngerThan,           // FileDateMode {fileCreatedMode,fileModifiedMode}
                     noDateSet,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     noCompareModeSet};

    enum fileTypeEntity {Folder, File, unresolved};

    enum copyMode{move,copy,noMode};

    QString nonIntervalString = "Enkel grænse", intervalString = "Interval grænse";
    QStringList intervalConditionalList {"Enkel grænse","Interval grænse"};

    // List variables..
    static QStringList sizeUnits(){return {"b","kb","mb","gb"};}


    /*
     * Mappings
     *      - actionMappings
     *      - conditionMappings
     *      - compareMappings
     *      - fileTypeFilterMappings
     */

    const QList<QPair<QString,ruleAction> > actionMappings {
        QPair<QString,ruleAction>(("Move content"),ruleAction::Move),
        QPair<QString,ruleAction>("Delete content",ruleAction::Delete),
        QPair<QString,ruleAction>("Copy content",ruleAction::Copy),
        QPair<QString,ruleAction>("Do nothing",ruleAction::none)};

    const QList<QPair<QString,ruleCriteria> > criteriaMappings {
        QPair<QString,ruleCriteria>("Base name",ruleCriteria::fileBaseMode),
        QPair<QString,ruleCriteria>("File name",ruleCriteria::filepathMode),
        QPair<QString,ruleCriteria>("File suffix",ruleCriteria::fileExtensionMode),
        QPair<QString,ruleCriteria>("File size",ruleCriteria::fileSize),
        QPair<QString,ruleCriteria>("In folder",ruleCriteria::fileParentMode),
        QPair<QString,ruleCriteria>("Date created",ruleCriteria::fileCreatedMode),
        QPair<QString,ruleCriteria>("Date edited",ruleCriteria::fileModifiedMode),
        QPair<QString,ruleCriteria>("No conditions",ruleCriteria::nonConditionalMode)};

    const QList<QPair<QString,ruleCompareCriteria> > compareCriteriaMappings {
        QPair<QString,ruleCompareCriteria>("Contains",ruleCompareCriteria::contains),
                QPair<QString,ruleCompareCriteria>("Does not contain",ruleCompareCriteria::dontMatch),
                QPair<QString,ruleCompareCriteria>("Matching",ruleCompareCriteria::match),
                QPair<QString,ruleCompareCriteria>("Does not match",ruleCompareCriteria::dontMatch),
                QPair<QString,ruleCompareCriteria>("Greater than",ruleCompareCriteria::greaterThan),
                QPair<QString,ruleCompareCriteria>("Greater or equal than",ruleCompareCriteria::greaterOrEqualThan),
                QPair<QString,ruleCompareCriteria>("Equal",ruleCompareCriteria::equal),
                QPair<QString,ruleCompareCriteria>("Less or equal than",ruleCompareCriteria::lesserOrEqualThan),
                QPair<QString,ruleCompareCriteria>("Equal than",ruleCompareCriteria::lesserThan),
                QPair<QString,ruleCompareCriteria>("Older than",ruleCompareCriteria::olderThan),
                QPair<QString,ruleCompareCriteria>("Extact date",ruleCompareCriteria::exactDate),
                QPair<QString,ruleCompareCriteria>("Younger than",ruleCompareCriteria::youngerThan)};

    const QList<QPair<QString,fileTypeEntity> > fileTypeFilterMappings
    {
        QPair<QString,fileTypeEntity>("Files", fileTypeEntity::File),
                QPair<QString,fileTypeEntity>("Folders", fileTypeEntity::Folder),
                QPair<QString,fileTypeEntity>("Undetermined", fileTypeEntity::unresolved)
    };

    // Retrieve list methods

    const QStringList buildStringListFromEntity(ruleType property = ruleType::all)
    {
        QStringList resultingList;
        if(property == ruleType::action || property == ruleType::all)
        {
            for ( QPair<QString,ruleAction> actionMapping : actionMappings)
                resultingList << actionMapping.first;
        }
        if(property == ruleType::criteria || property == ruleType::all)
        {
            for ( QPair<QString,ruleCriteria> pair : criteriaMappings)
                resultingList << pair.first;
        }
        if(property == ruleType::compareCriteria || property == ruleType::all)
        {
            for ( QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings)
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList fileCompareModesToStringList(ruleCriteria condition)
    {
        QStringList resultingList;
        if(condition == ruleCriteria::fileBaseMode ||
                condition == ruleCriteria::filepathMode ||
                condition == ruleCriteria::fileExtensionMode)
        {
            for(QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings) {
                if(pair.second == ruleCompareCriteria::contains ||
                        pair.second == ruleCompareCriteria::dontMatch ||
                        pair.second == ruleCompareCriteria::match ||
                        pair.second == ruleCompareCriteria::dontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == ruleCriteria::fileSize)
        {
            for(QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings)
            {
                if(pair.second == ruleCompareCriteria::greaterThan ||
                        pair.second ==ruleCompareCriteria::greaterOrEqualThan ||
                        pair.second ==ruleCompareCriteria::equal ||
                        pair.second ==ruleCompareCriteria::lesserOrEqualThan ||
                        pair.second == ruleCompareCriteria::lesserThan)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == ruleCriteria::fileCreatedMode ||
                condition == ruleCriteria::fileModifiedMode)
        {
            for(QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings)
            {
                if(pair.second == ruleCompareCriteria::olderThan ||
                        pair.second == ruleCompareCriteria::exactDate ||
                        pair.second == ruleCompareCriteria::youngerThan)
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

        for (QPair<QString,fileTypeEntity> pair : fileTypeFilterMappings)
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString fileActionEntityToString(const ruleAction mode)
    {
        for(QPair<QString,ruleAction> pair : actionMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    ruleAction fileActionEntityFromString(const QString mode)
    {
        for(QPair<QString,ruleAction> pair : actionMappings)
        {
            if(pair.first == mode)
                return pair.second;
        }
        return ruleAction::none;
    }

    QString fileConditionEntityToString(const ruleCriteria mode)
    {
        for(QPair<QString,ruleCriteria> pair : criteriaMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    ruleCriteria buildCriteriaFromString(const QString string)
    {
        for(QPair<QString,ruleCriteria> pair : criteriaMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return ruleCriteria::notDefined;
    }

    QString buildStringFromCompareCriteria(const ruleCompareCriteria mode)
    {
        for(QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings)
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    ruleCompareCriteria fileCompareEntityFromString(const QString string)
    {
        for(QPair<QString,ruleCompareCriteria> pair : compareCriteriaMappings)
        {
            if(pair.first == string)
                return pair.second;
        }
        return ruleCompareCriteria::noCompareModeSet;
    }

    QString fileTypeEntityToString(fileTypeEntity type)
    {
        for (QPair<QString,fileTypeEntity> pair : fileTypeFilterMappings)
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    fileTypeEntity fileTypeEntityFromString(QString string)
    {
        for (QPair<QString,fileTypeEntity> pair : fileTypeFilterMappings)
        {
            if(pair.first == string)
                return pair.second;
        }

        return fileTypeEntity::unresolved;
    }
};
typedef ruleDefinitions rD;

#endif // RULEDEFINITIONS_H
