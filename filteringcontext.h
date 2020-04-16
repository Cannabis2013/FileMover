#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include "ifiltereringcontext.h"
#include "filemodelbuilder.h"
#include "filelistservice.h"
#include "rulescontext.h"
#include "entitymodel.h"
#include "ientitymodelbuilder.h"

typedef IFileListService<IModelBuilder<IFileModel<QFileInfo,QUuid>,QString>> IDefaultListService;
typedef IFiltereringContext<DefaultRuleInterface,IFileModel<QFileInfo,QUuid>,IDefaultListService> DefaultFilteringContextInterface;
typedef IFileModel<QFileInfo,QUuid> DefaultFileModelInterface;

class FilteringContext :
        public DefaultFilteringContextInterface{
public:
    FilteringContext(IEntityModelBuilder<DefaultModelInterface,DefaultFileModelList> *service)
    {
        _entityModelBuilderService = service;
    }
    QList<const DefaultFileModelInterface*> process(const QList<const DefaultRuleInterface*> rules)
    {
        QList<DefaultFileModelInterface*> resultingList;
        auto filteredList = listService()->buildFileModels(RulesContext::All);

        for (auto rule : rules) {
            QList<const DefaultFileModelInterface*> list;
            for (auto criteria : rule->conditions()) {
                list = processFiles(criteria,filteredList,rule);
            }

        }
        return filteredList;
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
    QList<const DefaultFileModelInterface*> processFiles(const DefaultRuleCriteria *ruleCriteria,
                                      const QList<const IFileModel<QFileInfo,QUuid>*> &filteredList,const DefaultRuleInterface *rule)
    {
        QList<const IFileModel<QFileInfo,QUuid>*> resultingList, newFilteredList;
        for (auto model : filteredList)
        {
            // Check if current file is located within the domain of current rule
            auto appliesToPath = rule->appliesToPath();
            auto filepath = model->filepath();

            if(appliesToPath != "All")
            {
                if(!filepath.contains(appliesToPath))
                    continue;
            }

            auto fileInfo = model->fileInterface();
            if(ruleCriteria->criteria() == RulesContext::FileBaseMode)
            {
                auto match = ruleCriteria->compareCriteria() == RulesContext::Match;

                if(fileInfo.isFile())
                {
                    auto subject = fileInfo.baseName();
                    auto keywords = ruleCriteria->keywords();
                    if(compareStrings(keywords,subject,match))
                        resultingList << model;
                }
                else if(fileInfo.isDir())
                {
                    auto subject = fileInfo.fileName();
                    auto keywords = ruleCriteria->keywords();
                    if(compareStrings(keywords,subject,match))
                        resultingList << model;
                    else
                        resultingList << processFiles(ruleCriteria,model->children(),rule);
                }
            }
            else if(ruleCriteria->criteria() == RulesContext::FileNameMode ||
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
                        resultingList << processFiles(ruleCriteria,model->children(),rule);
                }

            }
            else if(ruleCriteria->criteria() == RulesContext::FileSizeMode &&
                    ruleCriteria->compareCriteria() != RulesContext::Interval)
            {
                auto sizeObject = ruleCriteria->sizeLimit();
                auto sizeUnits = sizeObject.first;
                auto sizeDataStorageUnit = sizeObject.second;
                auto bytes = FCU::convertToBytes(sizeUnits,sizeDataStorageUnit);

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

                auto lowerByteLimit = FCU::convertToBytes(lowerIntervalSizeUnits,lowerIntervalSizeDataStorageUnit);
                auto upperByteLimit = FCU::convertToBytes(upperIntervalSizeUnits,upperIntervalSizeDataStorageUnit);

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

    bool compareStrings(const QStringList &strings, const QString &object, const bool &match = false)
    {
        for (auto string : strings) {
            if(match && string == object)
                return true;
            else if(!match && string.contains(object))
                return true;
        }

        return false;
    }

    const QList<const DefaultFileModelInterface*> subtractList(const QList<DefaultFileModelInterface*> &source, const QList<DefaultFileModelInterface*> &selectedList)
    {
        QList<const DefaultFileModelInterface*> resultingList;
        for (auto sModel : source) {
            for (auto model : selectedList) {
                if(sModel->id() != model->id())
                    resultingList << sModel;
            }
        }

        return resultingList;
    }

    IDefaultListService* _listService;
    IEntityModelBuilder<DefaultModelInterface,DefaultFileModelList> *_entityModelBuilderService;

};

#endif // FILTERINGCONTEXT_H
