#ifndef FILELISTBUILDER_H
#define FILELISTBUILDER_H

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdiriterator.h>
#include "ifilelistservice.h"

class FileListService : public IFileListService<IModelBuilder<IFileModel<>,QString>>
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
                if(filter == FilesContext::All)
                    result << file.absoluteFilePath();
                else if(filter == FilesContext::File && file.isFile())
                    result << file.absoluteFilePath();
                else if(filter == FilesContext::Folder && file.isDir())
                    result << file.absoluteFilePath();
            }
        }

        return result;
    }

    DefaultFileModelList buildFileModels(const int &filter = FilesContext::All, const QStringList &paths = QStringList()) override
    {
        DefaultFileModelList resultingList;

        auto filepaths = paths == QStringList() ? _filepaths : paths;

        for (auto path : paths) {
            QDirIterator iterator(path,QDir::AllEntries | QDir::NoDotAndDotDot);
            while(iterator.hasNext())
            {
                QFileInfo file = iterator.next();
                if(file.isFile())
                    resultingList << builderService->buildModel(new QString(file.absoluteFilePath()));
                else if(file.isDir())
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
                                                     QString &str,
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
                                                         const int &mode = FilesContext::LesserThan) override
    {
        DefaultFileModelList result;
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = fileInfo.size();
                if(mode == FilesContext::LesserThan && subject < size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::LesserOrEqualThan && subject <= size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::Equal && subject == size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::greaterOrEqualThan && subject >= size)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::greaterThan && subject > size)
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
                                                  const int &mode = FilesContext::YoungerThan,
                                                  const int &dateMode = FilesContext::DateCreated) override
    {
        DefaultFileModelList result;
        auto compare = QDateTime(QDate(year,month,day));
        for (auto model : fileModelDelegates) {
            auto fileInfo = model->fileInterface();
            if(fileInfo.isFile())
            {
                auto subject = dateMode == FilesContext::DateCreated ? fileInfo.birthTime() :
                                                                       fileInfo.lastModified();

                if(mode == FilesContext::YoungerThan && subject < compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::YoungerOrExactThan && subject <= compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::Exact && subject == compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::OlderOrExtactThan && subject >= compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
                else if(mode == FilesContext::OlderThan && subject > compare)
                    result <<  modelBuilderService()->buildModel(new QString(model->filepath()));
            }
            else
            {
                result << filterFileModelsAccordingToDate(model->children(),day,month,year,mode);
            }
        }
        return result;
    }

    IModelBuilder<IFileModel<QFileInfo>,QString> *modelBuilderService() override
    {
        return builderService;
    }
    void setModelBuilderService(IModelBuilder<IFileModel<QFileInfo>,QString> *service) override
    {
        builderService = service;
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

    IModelBuilder<IFileModel<QFileInfo>,QString> *builderService;
    QStringList _filepaths;
};

#endif // FILELISTBUILDER_H
