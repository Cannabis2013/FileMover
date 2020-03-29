#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include "ifiltereringcontext.h"
#include "filemodelbuilder.h"
#include "filelistservice.h"
#include "rulescontext.h"
#include "entitymodel.h"

typedef IFileListService<IModelBuilder<IFileModel<>,QString>> IDefaultListService;
typedef IFiltereringContext<IDefaultRule,FileRuleDelegate,IDefaultListService> IDefaultFilteringContext;

class FilteringContext :
        public IDefaultFilteringContext{
public:
    QList<FileRuleDelegate*> process(const QList<const IDefaultRule*> objects)
    {
        auto compareStrings = [](const QStringList &strings, const QString &subject, const bool &match = true)->bool
        {
            for (auto string : strings) {
                if(match && string == subject)
                    return true;
                else if(!match && string.contains(subject))
                    return true;
            }

            return false;
        };
        QList<FileRuleDelegate*> resultingList;
        auto filteredList = listService()->buildFileModels(RulesContext::All);

        for (auto object : objects) {
            for (auto criteria : object->conditions()) {
                filteredList = processFiles(criteria,filteredList,compareStrings);
            }
            resultingList << DelegateBuilder::buildFileActionEntity<EntityModel>(listService()->fileLists(),
                                                                                      filteredList,
                                                                                      object->actionRuleEntity(),
                                                                                      object->destinationPaths());
        }
        return resultingList;
    }

    void setListService(IDefaultListService *service)
    {
        this->_listService = service;
    }
    IDefaultListService *listService()
    {
        return _listService;
    }

private:

    QList<const IFileModel<>*> processFiles(const IDefaultRuleCondition *ruleCriteria,
                                      const QList<const IFileModel<>*> &filteredList,
                                      bool (*compareStrings)(const QStringList&,const QString&,const bool&))
    {
        QList<const IFileModel<>*> resultingList;
        for (auto model : filteredList)
        {

            auto fileInfo = model->fileInterface();

            if(ruleCriteria->criteria() == RulesContext::FileNameMode ||
                    ruleCriteria->criteria() == RulesContext::FileExtensionMode)
            {
                auto isSuffix = ruleCriteria->criteria() == RulesContext::FileNameMode ? false : true;
                auto match = ruleCriteria->compareCriteria() == RulesContext::Match;

                if(fileInfo.isFile())
                {
                    auto subject = isSuffix ? fileInfo.suffix() : fileInfo.fileName();
                    auto keywords = ruleCriteria->keywords();
                    if(compareStrings(keywords,subject,match))
                        resultingList << model;
                }
                else if(fileInfo.isDir() && ruleCriteria->criteria() != RulesContext::FileExtensionMode)
                {
                    auto subject = fileInfo.fileName();
                    auto keywords = ruleCriteria->keywords();
                    if(compareStrings(keywords,subject,match))
                        resultingList << model;
                    else
                        resultingList << processFiles(ruleCriteria,model->children(),compareStrings);
                }

            }
            else if(ruleCriteria->criteria() == RulesContext::FileSizeMode &&
                    ruleCriteria->compareCriteria() != RulesContext::Interval)
            {
                auto sizeObject = ruleCriteria->sizeLimit();
                auto sizeUnits = sizeObject.first;
                auto sizeDataStorageUnit = sizeObject.second;
                auto bytes = FilesContext::convertToBytes(sizeUnits,sizeDataStorageUnit);

                if((ruleCriteria->compareCriteria() == RulesContext::LesserThan && fileInfo.size() < bytes) ||
                        (ruleCriteria->compareCriteria() == RulesContext::LesserOrEqualThan && fileInfo.size() <= bytes) ||
                        (ruleCriteria->compareCriteria() == RulesContext::Equal && fileInfo.size() == bytes) ||
                        (ruleCriteria->compareCriteria() == RulesContext::GreaterOrEqualThan  && fileInfo.size() >= bytes) ||
                        (ruleCriteria->compareCriteria() == RulesContext::GreaterThan && fileInfo.size() < bytes))
                {
                    resultingList << model;
                }
            }
            else if(ruleCriteria->criteria() == RulesContext::FileSizeMode &&
                    ruleCriteria->compareCriteria() == RulesContext::Interval)
            {
                auto subject = fileInfo.size();
                auto sizeIntervalObject = ruleCriteria->sizeInterval();
                auto lowerIntervalObject = sizeIntervalObject.first;
                auto upperIntervalObject = sizeIntervalObject.second;

                auto lowerIntervalSizeUnits = lowerIntervalObject.first;
                auto lowerIntervalSizeDataStorageUnit = lowerIntervalObject.second;

                auto upperIntervalSizeUnits = upperIntervalObject.first;
                auto upperIntervalSizeDataStorageUnit = upperIntervalObject.second;

                auto lowerByteLimit = FilesContext::convertToBytes(lowerIntervalSizeUnits,lowerIntervalSizeDataStorageUnit);
                auto upperByteLimit = FilesContext::convertToBytes(upperIntervalSizeUnits,upperIntervalSizeDataStorageUnit);

                if(subject >= lowerByteLimit && subject  <= upperByteLimit)
                    resultingList << model;

            }
            else if((ruleCriteria->criteria() == RulesContext::FileCreatedMode ||
                     ruleCriteria->criteria() == RulesContext::FileModifiedMode) &&
                    ruleCriteria->compareCriteria() != RulesContext::Interval)
            {
                auto subject = ruleCriteria->criteria() == RulesContext::FileCreatedMode ? fileInfo.birthTime() :
                                                                                           fileInfo.lastModified();
                auto date = ruleCriteria->date();

                if((ruleCriteria->compareCriteria() == RulesContext::YoungerThan && subject < date) ||
                        (ruleCriteria->compareCriteria() == RulesContext::ExactDate && subject == date) ||
                        (ruleCriteria->compareCriteria() == RulesContext::OlderThan && subject > date))
                {
                    resultingList << model;
                }
            }
            else if((ruleCriteria->criteria() == RulesContext::FileCreatedMode ||
                     ruleCriteria->criteria() == RulesContext::FileModifiedMode) &&
                    ruleCriteria->compareCriteria() == RulesContext::Interval)
            {
                auto subject = ruleCriteria->criteria() == RulesContext::FileCreatedMode ? fileInfo.birthTime() :
                                                                                           fileInfo.lastModified();
                auto oldestDate = ruleCriteria->dateIntervals().first;
                auto earliestDate = ruleCriteria->dateIntervals().second;

                if(subject < oldestDate && subject > earliestDate)
                    resultingList << model;
            }
            else if(ruleCriteria->criteria() == RulesContext::FileParentMode)
            {
                auto parentModel = model->parent();
                auto fileInfo =parentModel->fileInterface();
                auto subject = fileInfo.fileName();
                auto keywords = ruleCriteria->keywords();
                if(compareStrings(keywords,subject,true))
                    resultingList << model;
            }
        }
        return resultingList;
    }

    IDefaultListService* _listService;

};

#endif // FILTERINGCONTEXT_H
