#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

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

class RuleDefinitions : public IRuleDefinitions
{
public:
    // Enumerated variables..
    QString nonIntervalString = "Enkel grænse", intervalString = "Interval grænse";
    QStringList intervalMode {"Enkel grænse","Interval grænse"};

    // List variables..

    const QStringList intervalConditionalList() override
    {
        return intervalMode;
    }

    QStringList sizeUnits() override {return {"b","kb","mb","gb"};}


    /*
     * Mapping methods
     *      - actionMappings
     *      - conditionMappings
     *      - compareMappings
     *      - fileTypeFilterMappings
     */


    // Retrieve list methods

    QString buildDefaultStringValue() override
    {
        return "No criterias";
    }

    const QStringList buildStringListFromEntity(const int &property) override
    {
        QStringList resultingList;
        if(property == DefaultRulesContext::Action || property == DefaultRulesContext::All)
        {
            for ( QPair<QString,DefaultRulesContext::RuleAction> actionMapping : actionMappings())
                resultingList << actionMapping.first;
        }
        if(property == DefaultRulesContext::Condition || property == DefaultRulesContext::All)
        {
            for ( QPair<QString,DefaultRulesContext::RuleCriteria> pair : criteriaMappings())
                resultingList << pair.first;
        }
        if(property == DefaultRulesContext::CompareCriteria || property == DefaultRulesContext::All)
        {
            for ( QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList fileCompareModesToStringList(const int &condition) override
    {
        QStringList resultingList;
        if(condition == DefaultRulesContext::FileBaseMode ||
                condition == DefaultRulesContext::FileNameMode ||
                condition == DefaultRulesContext::FileExtensionMode)
        {
            for(QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings()) {
                if(pair.second == DefaultRulesContext::Contain ||
                        pair.second == DefaultRulesContext::DontMatch ||
                        pair.second == DefaultRulesContext::Match ||
                        pair.second == DefaultRulesContext::DontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == DefaultRulesContext::FileSizeMode)
        {
            for(QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == DefaultRulesContext::GreaterThan ||
                        pair.second ==DefaultRulesContext::GreaterOrEqualThan ||
                        pair.second ==DefaultRulesContext::Equal ||
                        pair.second ==DefaultRulesContext::LesserOrEqualThan ||
                        pair.second == DefaultRulesContext::LesserThan)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == DefaultRulesContext::FileCreatedMode ||
                condition == DefaultRulesContext::FileModifiedMode)
        {
            for(QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == DefaultRulesContext::OlderThan ||
                        pair.second == DefaultRulesContext::ExactDate ||
                        pair.second == DefaultRulesContext::YoungerThan)
                {
                    resultingList << pair.first;
                }
            }
        }

        return resultingList;
    }

    const QStringList allFileTypeEntitiesToStrings() override
    {
        QStringList resultingList;

        for (QPair<QString,DefaultRulesContext::FileType> pair : fileTypeFilterMappings())
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString fileActionEntityToString(const int &mode) override
    {
        for(QPair<QString,DefaultRulesContext::RuleAction> pair : actionMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int fileActionEntityFromString(const QString mode) override
    {
        for(QPair<QString,DefaultRulesContext::RuleAction> pair : actionMappings())
        {
            if(pair.first == mode)
                return pair.second;
        }
        return DefaultRulesContext::NoAction;
    }

    QString buildStringFromCriteria(const int &mode) override
    {
        for(QPair<QString,DefaultRulesContext::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int buildCriteriaFromString(const QString string) override
    {
        for(QPair<QString,DefaultRulesContext::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return DefaultRulesContext::NotDefined;
    }

    QString buildStringFromCompareCriteria(const int &mode) override
    {
        for(QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int fileCompareEntityFromString(const QString string) override
    {
        for(QPair<QString,DefaultRulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return DefaultRulesContext::NoCompareModeSet;
    }

    QString fileTypeCriteriaToString(const int &type) override
    {
        for (QPair<QString,DefaultRulesContext::FileType> pair : fileTypeFilterMappings())
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    int fileTypeCriteriaFromString(const QString &string) override
    {
        for (QPair<QString,DefaultRulesContext::FileType> pair : fileTypeFilterMappings())
        {
            if(pair.first == string)
                return pair.second;
        }

        return DefaultRulesContext::Unresolved;
    }

private:
    const QList<QPair<QString,DefaultRulesContext::RuleAction> > actionMappings() {
        return {
            QPair<QString,DefaultRulesContext::RuleAction>(("Move content"),DefaultRulesContext::RuleAction::MoveAction),
            QPair<QString,DefaultRulesContext::RuleAction>("Delete content",DefaultRulesContext::DeleteAction),
            QPair<QString,DefaultRulesContext::RuleAction>("Copy content",DefaultRulesContext::CopyAction),
            QPair<QString,DefaultRulesContext::RuleAction>("Do nothing",DefaultRulesContext::NoAction)};
        }

    const QList<QPair<QString,DefaultRulesContext::RuleCriteria> > criteriaMappings() {
        return {
            QPair<QString,DefaultRulesContext::RuleCriteria>("Base name",DefaultRulesContext::FileBaseMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("File name",DefaultRulesContext::FileNameMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("File suffix",DefaultRulesContext::FileExtensionMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("File size",DefaultRulesContext::FileSizeMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("In folder",DefaultRulesContext::FileParentMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("Date created",DefaultRulesContext::FileCreatedMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("Date edited",DefaultRulesContext::FileModifiedMode),
            QPair<QString,DefaultRulesContext::RuleCriteria>("No conditions",DefaultRulesContext::NonConditionalMode)};
        }

    const QList<QPair<QString,DefaultRulesContext::RuleCompareCriteria> > compareCriteriaMappings(){

        return {
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Contains",DefaultRulesContext::Contain),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Does not contain",DefaultRulesContext::DontMatch),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Matching",DefaultRulesContext::Match),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Does not match",DefaultRulesContext::DontMatch),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Greater than",DefaultRulesContext::GreaterThan),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Greater or equal than",DefaultRulesContext::GreaterOrEqualThan),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Equal",DefaultRulesContext::Equal),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Less or equal than",DefaultRulesContext::LesserOrEqualThan),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Equal than",DefaultRulesContext::LesserThan),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Older than",DefaultRulesContext::OlderThan),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Extact date",DefaultRulesContext::ExactDate),
            QPair<QString,DefaultRulesContext::RuleCompareCriteria>("Younger than",DefaultRulesContext::YoungerThan)};
        }

    const QList<QPair<QString,DefaultRulesContext::FileType> > fileTypeFilterMappings()
    {
        return {
            QPair<QString,DefaultRulesContext::FileType>("Files", DefaultRulesContext::File),
            QPair<QString,DefaultRulesContext::FileType>("Folders", DefaultRulesContext::Folder),
            QPair<QString,DefaultRulesContext::FileType>("Undetermined", DefaultRulesContext::Unresolved)
        };
    };
};



#endif // RULEDEFINITIONS_H
