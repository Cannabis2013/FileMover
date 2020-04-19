#ifndef FILELISTBUILDER_H
#define FILELISTBUILDER_H

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdiriterator.h>
#include "ifilelistservice.h"

class FileListService : public IDefaultFileListService
{
    // IFileListBuilder interface
public:
    QStringList allFiles(const QStringList &paths, const int &filter) override
    {
        QStringList result;
        for (auto path : paths) {
            QDirIterator iterator(path,QDir::AllEntries | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
            while(iterator.hasNext())
            {
                QFileInfo file = iterator.next();
                if(filter == FileFilteringContext::All)
                    result << file.absoluteFilePath();
                else if(filter == FileFilteringContext::File && file.isFile())
                    result << file.absoluteFilePath();
                else if(filter == FileFilteringContext::Folder && file.isDir())
                    result << file.absoluteFilePath();
            }
        }

        return result;
    }

    DefaultFileModelList buildFileModels(const QStringList &paths = QStringList(), const int &filter = FileFilteringContext::All) override
    {
        DefaultFileModelList resultingList;

        auto filepaths = (paths == QStringList()) ? _filepaths : paths;

        for (auto path : filepaths)
        {
            QDirIterator iterator(path,QDir::AllEntries | QDir::NoDotAndDotDot);
            while(iterator.hasNext())
            {
                QFileInfo file = iterator.next();
                if(file.isFile() && filter != FileFilteringContext::Folder)
                    resultingList << builderService->buildModel(new QString(file.absoluteFilePath()));
                else if(file.isDir() && filter != FileFilteringContext::File)
                {
                    auto folder = builderService->buildModel(new QString(file.absoluteFilePath()));
                    auto folderContent = modelsFromFolder(file.absoluteFilePath());
                    folder->setChildren(folderContent);
                    resultingList << folder;
                }
            }
        }

            return resultingList;
    }

    DefaultFileModelList filterFileModelsThatMatch(const DefaultFileModelList &fileModelDelegates,
                                                   QString &str,
                                                   bool dontMatch = false,
                                                   bool suffix = false) override
    {
        DefaultFileModelList result;
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = suffix ? fileInfo.suffix() : fileInfo.fileName();
                if(!dontMatch && subject == str)
                    result << builderService->buildModel(new QString(fileInfo.absoluteFilePath()));
                else if(dontMatch && subject != str)
                    result << builderService->buildModel(new QString(fileInfo.absoluteFilePath()));
            }
            else
            {
                auto subject = fileInfo.fileName();
                if(!dontMatch && subject == str)
                    result << result << builderService->buildModel(new QString(fileInfo.absoluteFilePath()));
                else if(dontMatch && subject != str)
                    result << builderService->buildModel(new QString(fileInfo.absoluteFilePath()));
                else
                    result << result << builderService->buildModel(new QString(fileInfo.absoluteFilePath()));
            }
        }
        return result;
    }

    DefaultFileModelList filterFileModelsThatContain(const DefaultFileModelList &fileModelDelegates,
                                                     const QString &str,
                                                     bool dontContain = false,
                                                     bool suffix = false) override
    {
        DefaultFileModelList result;
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = suffix ? fileInfo.suffix() : fileInfo.fileName();
                if(!dontContain && subject.contains(str))
                    result << modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(dontContain && !subject.contains(str))
                    result << modelBuilderService()->buildModel(new QString(model->filepath()));
            }
            else
            {
                auto subject = fileInfo.fileName();
                if(!dontContain && subject.contains(str))
                    result << modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(dontContain && !subject.contains(str))
                    result << modelBuilderService()->buildModel(new QString(model->filepath()));
                else
                    result << modelBuilderService()->buildModel(new QString(model->filepath()));
            }
        }
        return result;
    }

    DefaultFileModelList filterFileModelsAccordingToSize(const DefaultFileModelList &fileModelDelegates,
                                                         const int &size,
                                                         const int &mode = FileFilteringContext::LesserThan) override
    {
        DefaultFileModelList result;
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = fileInfo.size();
                if(mode == FileFilteringContext::LesserThan && subject < size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::LesserOrEqualThan && subject <= size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::Equal && subject == size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::GreaterOrEqualThan && subject >= size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::GreaterThan && subject > size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
            }
            else
            {
                result << filterFileModelsAccordingToSize(model->children(),size,mode);
            }
        }
        return result;
    }

    DefaultFileModelList filterFileModelsAccordingToDate(const DefaultFileModelList &fileModelDelegates,
                                                  const int &day,
                                                  const int &month,
                                                  const int &year,
                                                  const int &mode = FileFilteringContext::YoungerThan,
                                                  const int &dateMode = FileFilteringContext::DateCreated) override
    {
        DefaultFileModelList result;
        auto compare = QDateTime(QDate(year,month,day));
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = dateMode == FileFilteringContext::DateCreated ? fileInfo.birthTime() :
                                                                       fileInfo.lastModified();

                if(mode == FileFilteringContext::YoungerThan && subject < compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::Exact && subject == compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FileFilteringContext::OlderThan && subject > compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
            }
            else
            {
                result << filterFileModelsAccordingToDate(model->children(),day,month,year,mode);
            }
        }
        return result;
    }

    IModelBuilder<IFileModel<QFileInfo,QUuid>,QString> *modelBuilderService() override
    {
        return builderService;
    }
    IFileListService<IModelBuilder<IFileModel<QFileInfo,QUuid>,QString>>* setModelBuilderService(IModelBuilder<IFileModel<QFileInfo,QUuid>,QString> *service) override
    {
        builderService = service;
        return this;
    }

    void appendFileLists(const QStringList &filepaths) override
    {
        _filepaths = filepaths;
    }
    QStringList fileLists() override
    {
        return _filepaths;
    }

private:

    const QStringList modelsToStringList(const DefaultFileModelList &models)
    {
        QStringList result;
        for (auto model : models) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
                result << fileInfo.absoluteFilePath();
            else
                result << fileInfo.absoluteFilePath() << modelsToStringList(model->children());
        }
        return result;
    }

    DefaultFileModelList modelsFromFolder(const QString &path)
    {
        DefaultFileModelList result;
        QDirIterator iterator(path);
        while (iterator.hasNext()) {
            QFileInfo file = iterator.next();
            if(file.isFile())
                result << builderService->buildModel(new QString(file.absoluteFilePath()));
            else if(file.isDir())
            {
                auto delegate = builderService->buildModel(new QString(file.absoluteFilePath()));
                auto children = modelsFromFolder(file.absoluteFilePath());
                delegate->setChildren(children);
                result << delegate;
            }
        }
        return result;
    }

    IModelBuilder<IFileModel<QFileInfo,QUuid>,QString> *builderService;
    QStringList _filepaths;
};

#endif // FILELISTBUILDER_H
