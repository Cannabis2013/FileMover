#ifndef RULEDEFINITIONS_H
#define RULEDEFINITIONS_H

#include <qtranslator.h>

#include "iruledefinitions.h"
#include "rulescontext.h"

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
    QString appliesToAll = "All";
    // List variables..

    QString defaultAppliesToAll() override
    {
        return appliesToAll;
    }

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
        if(property == RulesContext::Action || property == RulesContext::All)
        {
            for ( QPair<QString,RulesContext::RuleAction> actionMapping : actionMappings())
                resultingList << actionMapping.first;
        }
        if(property == RulesContext::Condition || property == RulesContext::All)
        {
            for ( QPair<QString,RulesContext::RuleCriteria> pair : criteriaMappings())
                resultingList << pair.first;
        }
        if(property == RulesContext::CompareCriteria || property == RulesContext::All)
        {
            for ( QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
                resultingList << pair.first;
        }

        return resultingList;
    }

    const QStringList fileCompareModesToStringList(const int &condition) override
    {
        QStringList resultingList;
        if(condition == RulesContext::FileBaseMode ||
                condition == RulesContext::FileNameMode ||
                condition == RulesContext::FileExtensionMode)
        {
            for(QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings()) {
                if(pair.second == RulesContext::Contain ||
                        pair.second == RulesContext::DontMatch ||
                        pair.second == RulesContext::Match ||
                        pair.second == RulesContext::DontMatch)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == RulesContext::FileSizeMode)
        {
            for(QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == RulesContext::GreaterThan ||
                        pair.second ==RulesContext::GreaterOrEqualThan ||
                        pair.second ==RulesContext::Equal ||
                        pair.second ==RulesContext::LesserOrEqualThan ||
                        pair.second == RulesContext::LesserThan)
                {
                    resultingList << pair.first;
                }
            }
        }
        else if(condition == RulesContext::FileCreatedMode ||
                condition == RulesContext::FileModifiedMode)
        {
            for(QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
            {
                if(pair.second == RulesContext::OlderThan ||
                        pair.second == RulesContext::ExactDate ||
                        pair.second == RulesContext::YoungerThan)
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

        for (QPair<QString,RulesContext::FileType> pair : fileTypeFilterMappings())
            resultingList << pair.first;

        return resultingList;
    }

    // From type1 to type2 methods

    QString fileActionEntityToString(const int &mode) override
    {
        for(QPair<QString,RulesContext::RuleAction> pair : actionMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int fileActionEntityFromString(const QString mode) override
    {
        for(QPair<QString,RulesContext::RuleAction> pair : actionMappings())
        {
            if(pair.first == mode)
                return pair.second;
        }
        return RulesContext::NoAction;
    }

    QString buildStringFromCriteria(const int &mode) override
    {
        for(QPair<QString,RulesContext::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int buildCriteriaFromString(const QString string) override
    {
        for(QPair<QString,RulesContext::RuleCriteria> pair : criteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return RulesContext::NotDefined;
    }

    QString buildStringFromCompareCriteria(const int &mode) override
    {
        for(QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.second == mode)
                return pair.first;
        }
        return QString();
    }

    int fileCompareEntityFromString(const QString string) override
    {
        for(QPair<QString,RulesContext::RuleCompareCriteria> pair : compareCriteriaMappings())
        {
            if(pair.first == string)
                return pair.second;
        }
        return RulesContext::NoCompareModeSet;
    }

    QString fileTypeCriteriaToString(const int &type) override
    {
        for (QPair<QString,RulesContext::FileType> pair : fileTypeFilterMappings())
        {
            if(pair.second == type)
                return pair.first;
        }

        return QString();
    }

    int fileTypeCriteriaFromString(const QString &string) override
    {
        for (QPair<QString,RulesContext::FileType> pair : fileTypeFilterMappings())
        {
            if(pair.first == string)
                return pair.second;
        }

        return RulesContext::Unresolved;
    }

private:
    const QList<QPair<QString,RulesContext::RuleAction> > actionMappings() {
        return {
            QPair<QString,RulesContext::RuleAction>(("Move content"),RulesContext::RuleAction::MoveAction),
            QPair<QString,RulesContext::RuleAction>("Delete content",RulesContext::DeleteAction),
            QPair<QString,RulesContext::RuleAction>("Copy content",RulesContext::CopyAction),
            QPair<QString,RulesContext::RuleAction>("Do nothing",RulesContext::NoAction)};
        }

    const QList<QPair<QString,RulesContext::RuleCriteria> > criteriaMappings() {
        return {
            QPair<QString,RulesContext::RuleCriteria>("Base name",RulesContext::FileBaseMode),
            QPair<QString,RulesContext::RuleCriteria>("File name",RulesContext::FileNameMode),
            QPair<QString,RulesContext::RuleCriteria>("File suffix",RulesContext::FileExtensionMode),
            QPair<QString,RulesContext::RuleCriteria>("File size",RulesContext::FileSizeMode),
            QPair<QString,RulesContext::RuleCriteria>("In folder",RulesContext::FileParentMode),
            QPair<QString,RulesContext::RuleCriteria>("Date created",RulesContext::FileCreatedMode),
            QPair<QString,RulesContext::RuleCriteria>("Date edited",RulesContext::FileModifiedMode),
            QPair<QString,RulesContext::RuleCriteria>("No conditions",RulesContext::NonConditionalMode)};
        }

    const QList<QPair<QString,RulesContext::RuleCompareCriteria> > compareCriteriaMappings(){

        return {
            QPair<QString,RulesContext::RuleCompareCriteria>("Contains",RulesContext::Contain),
            QPair<QString,RulesContext::RuleCompareCriteria>("Does not contain",RulesContext::DontMatch),
            QPair<QString,RulesContext::RuleCompareCriteria>("Matching",RulesContext::Match),
            QPair<QString,RulesContext::RuleCompareCriteria>("Does not match",RulesContext::DontMatch),
            QPair<QString,RulesContext::RuleCompareCriteria>("Greater than",RulesContext::GreaterThan),
            QPair<QString,RulesContext::RuleCompareCriteria>("Greater or equal than",RulesContext::GreaterOrEqualThan),
            QPair<QString,RulesContext::RuleCompareCriteria>("Equal",RulesContext::Equal),
            QPair<QString,RulesContext::RuleCompareCriteria>("Less or equal than",RulesContext::LesserOrEqualThan),
            QPair<QString,RulesContext::RuleCompareCriteria>("Equal than",RulesContext::LesserThan),
            QPair<QString,RulesContext::RuleCompareCriteria>("Older than",RulesContext::OlderThan),
            QPair<QString,RulesContext::RuleCompareCriteria>("Extact date",RulesContext::ExactDate),
            QPair<QString,RulesContext::RuleCompareCriteria>("Younger than",RulesContext::YoungerThan)};
        }

    const QList<QPair<QString,RulesContext::FileType> > fileTypeFilterMappings()
    {
        return {
            QPair<QString,RulesContext::FileType>("Files", RulesContext::File),
            QPair<QString,RulesContext::FileType>("Folders", RulesContext::Folder),
            QPair<QString,RulesContext::FileType>("Undetermined", RulesContext::Unresolved)
        };
    };
};



#endif // RULEDEFINITIONS_H
