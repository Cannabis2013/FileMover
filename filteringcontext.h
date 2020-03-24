#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include "ifiltereringcontext.h"
#include "filemodelbuilder.h"
#include "filelistservice.h"
#include "rulescontext.h"

typedef IFileListService<IModelBuilder<IFileModel<>,QString>> IDefaultListService;

class FilteringContext :
        public IFiltereringContext<IDefaultRule,IFileModel<>,IDefaultListService>
{
public:
    FilteringContext()
    {
        _listService = new FileListService();
    }
    QList<IFileModel<> *> process(const QList<IDefaultRule*> objects, const QList<IFileModel<> *> subjects)
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

        QList<IFileModel<>*> filteredList = subjects, temporaryList;
        for (auto object : objects) {
            for (auto ruleCriteria : object->conditions()) {
                for (auto model : filteredList) {
                    if(ruleCriteria->criteria() == RulesContext::FileNameMode ||
                            ruleCriteria->criteria() == RulesContext::FileExtensionMode)
                    {
                        auto isSuffix = ruleCriteria->criteria() == RulesContext::FileNameMode ? false : true;
                        auto match = ruleCriteria->compareCriteria() == RulesContext::Match;

                        auto fileInfo = model->fileInterface();
                        auto subject = isSuffix ? fileInfo.suffix() : fileInfo.filePath();
                        auto keywords = ruleCriteria->keyWords();
                        if(compareStrings(keywords,subject,match))
                            temporaryList << model;
                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileSizeMode &&
                            ruleCriteria->compareCriteria() != RulesContext::Interval)
                    {
                        auto size = ruleCriteria->sizeLimit();

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileSizeMode &&
                            ruleCriteria->compareCriteria() == RulesContext::Interval)
                    {

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileCreatedMode &&
                            ruleCriteria->compareCriteria() != RulesContext::Interval)
                    {

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileCreatedMode &&
                            ruleCriteria->compareCriteria() == RulesContext::Interval)
                    {

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileModifiedMode &&
                            ruleCriteria->compareCriteria() != RulesContext::Interval)
                    {

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileModifiedMode &&
                            ruleCriteria->compareCriteria() == RulesContext::Interval)
                    {

                    }
                    else if(ruleCriteria->criteria() == RulesContext::FileParentMode)
                    {

                    }

                }

                filteredList = temporaryList;
                temporaryList.clear();
            }
        }

        return filteredList;
    }

public:
    void setListService(IDefaultListService *service)
    {
        this->_listService = service;
    }
    IDefaultListService *listService()
    {
        return _listService;
    }

private:
    IDefaultListService* _listService;

};

#endif // FILTERINGCONTEXT_H
