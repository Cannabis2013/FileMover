#ifndef FILELISTBUILDER_H
#define FILELISTBUILDER_H

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdiriterator.h>
#include "ifilelistservice.h"
#include "filemodeldelegate.h"

class FileListService : public IFileListService
{
    // IFileListBuilder interface
public:

    QStringList allFiles(const QStringList &paths, const int &filter)
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

    FileModelList fileModelDelegates(const QStringList &paths, const int &filter = FilesContext::All)
    {
        FileModelList resultingList;

        for (auto path : paths) {
            QDirIterator iterator(path,QDir::AllEntries | QDir::NoDotAndDotDot);
            while(iterator.hasNext())
            {
                QFileInfo file = iterator.next();
                if(file.isFile())
                    resultingList << FileModelDelegate<FileModel>::buildFileModelDelegate(file.absoluteFilePath());
                else if(file.isDir())
                {
                    auto folder = FileModelDelegate<FileModel>::buildFileModelDelegate(file.absoluteFilePath());
                    auto folderContent = modelsFromFolder(file.absoluteFilePath());
                    folder->setChildren(folderContent);
                    resultingList << folder;
                }
            }
        }

        return resultingList;
    }

    FileModelList filterFileModelsThatMatch(const FileModelList &fileModelDelegates, QString &str, bool dontMatch = false, bool suffix = false)
    {
        FileModelList result;
        for (auto fileModelDelegate : fileModelDelegates) {
            auto model = fileModelDelegate->model();
            if(model->isFile())
            {
                auto subject = suffix ? model->suffix() : model->fileName();
                if(!dontMatch && subject == str)
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(dontMatch && subject != str)
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
            }
            else
            {
                auto subject = model->fileName();
                if(!dontMatch && subject == str)
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(dontMatch && subject != str)
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else
                    result << filterFileModelsThatMatch(fileModelDelegate->children(),str,dontMatch,suffix);
            }
        }
        return result;
    }

    FileModelList filterFileModelsThatContain(const FileModelList &fileModelDelegates, QString &str, bool dontContain = false, bool suffix = false)
    {
        FileModelList result;
        for (auto fileModelDelegate : fileModelDelegates) {
            auto model = fileModelDelegate->model();
            if(model->isFile())
            {
                auto subject = suffix ? model->suffix() : model->fileName();
                if(!dontContain && subject.contains(str))
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(dontContain && !subject.contains(str))
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
            }
            else
            {
                auto subject = model->fileName();
                if(!dontContain && subject.contains(str))
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(dontContain && !subject.contains(str))
                    result << FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else
                    result << filterFileModelsThatMatch(fileModelDelegate->children(),str,dontContain,suffix);
            }
        }
        return result;
    }

    FileModelList filterFileModelsAccordingToSize(const FileModelList &fileModelDelegates, const int &size, const int &mode = FilesContext::LesserThan)
    {
        FileModelList result;
        for (auto fileModelDelegate : fileModelDelegates) {
            auto model = fileModelDelegate->model();
            if(model->isFile())
            {
                auto subject = model->size();
                if(mode == FilesContext::LesserThan && subject < size)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::LesserOrEqualThan && subject <= size)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::Equal && subject == size)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::greaterOrEqualThan && subject >= size)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::greaterThan && subject > size)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
            }
            else
            {
                result << filterFileModelsAccordingToSize(fileModelDelegate->children(),size,mode);
            }
        }
        return result;
    }

    FileModelList filterFileModelsAccordingToDate(const FileModelList &fileModelDelegates,
                                                  const int &day,
                                                  const int &month,
                                                  const int &year,
                                                  const int &mode = FilesContext::YoungerThan,
                                                  const int &dateMode = FilesContext::DateCreated)
    {
        FileModelList result;
        auto compare = QDateTime(QDate(year,month,day));
        for (auto fileModelDelegate : fileModelDelegates) {
            auto model = fileModelDelegate->model();
            if(model->isFile())
            {
                auto subject = dateMode == FilesContext::DateCreated ? model->birthTime() :
                                                                       model->lastModified();

                if(mode == FilesContext::YoungerThan && subject < compare)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::YoungerOrExactThan && subject <= compare)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::Exact && subject == compare)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::OlderOrExtactThan && subject >= compare)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
                else if(mode == FilesContext::OlderThan && subject > compare)
                    result <<  FileModelDelegate<FileModel>::buildFileModelDelegate(model->absoluteFilePath());
            }
            else
            {
                result << filterFileModelsAccordingToDate(fileModelDelegate->children(),day,month,year,mode);
            }
        }
        return result;
    }

    const QStringList modelsToStringList(const FileModelList &fileDelegates)
    {
        QStringList result;
        for (auto delegate : fileDelegates) {
            auto model = delegate->model();
            if(model->isFile())
                result << model->absoluteFilePath();
            else
                result << model->absoluteFilePath() << modelsToStringList(delegate->children());
        }

        return result;
    }

private:
    FileModelList modelsFromFolder(const QString &path)
    {
        FileModelList result;
        QDirIterator iterator(path);
        while (iterator.hasNext()) {
            QFileInfo file = iterator.next();
            if(file.isFile())
                result << FileModelDelegate<FileModel>::buildFileModelDelegate(file.absoluteFilePath());
            else if(file.isDir())
            {
                auto delegate = FileModelDelegate<FileModel>::buildFileModelDelegate(file.absoluteFilePath());
                auto children = modelsFromFolder(file.absoluteFilePath());
                delegate->setChildren(children);
                result << delegate;
            }
        }
        return result;
    }
};

#endif // FILELISTBUILDER_H
