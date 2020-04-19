#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "defaultfilemodelscontext.h"
#include "imodelbuilder.h"

namespace FileFilteringContext
{
    class IFileListService;

    enum sizeComparemode {LesserThan = 0x044,LesserOrEqualThan = 0x045, Equal = 0x046,GreaterOrEqualThan = 0x047,GreaterThan = 0x048,};
    enum dateCompareMode {OlderThan = 0x050, Exact = 0x051, YoungerThan = 0x052};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    enum FileType {Folder = 0x060, File = 0x061, All = Folder | File, unresolved = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}

typedef QList<const IFileModel<QFileInfo,QUuid>*> DefaultFileModelList;

template<class TModelBuilder>
class IFileListService
{
public:
    virtual QStringList allFiles(const QStringList &paths, const int &filter) = 0;
    virtual DefaultFileModelList buildFileModels(const QStringList &paths = QStringList(), const int &filter = FileFilteringContext::All) = 0;
    virtual DefaultFileModelList filterFileModelsThatMatch(const DefaultFileModelList &buildFileModels,
                                                           QString &str,
                                                           bool dontMatch = false,
                                                           bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsThatContain(const DefaultFileModelList &buildFileModels,
                                                             const QString &str,
                                                             bool dontContain = false,
                                                             bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToSize(const DefaultFileModelList &buildFileModels,
                                                                 const int &size,
                                                                 const int &mode = FileFilteringContext::LesserThan) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToDate(const DefaultFileModelList &buildFileModels,
                                                          const int &day,
                                                          const int &month,
                                                          const int &year,
                                                          const int &mode = FileFilteringContext::YoungerThan,
                                                          const int &dateMode = FileFilteringContext::DateCreated) = 0;

    virtual void appendFileLists(const QStringList &filepaths) = 0;
    virtual QStringList fileLists() = 0;

    virtual TModelBuilder *modelBuilderService() = 0;
    virtual IFileListService<TModelBuilder> *setModelBuilderService(TModelBuilder *service) = 0;
};

typedef IFileListService<IModelBuilder<IFileModel<QFileInfo,QUuid>,QString>> IDefaultFileListService;


#endif // IFILELISTBUILDER_H
