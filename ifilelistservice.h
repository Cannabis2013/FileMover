#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "defaultfilemodelscontext.h"
#include "imodelbuilder.h"

namespace FilesContext
{
    enum sizeComparemode {LesserThan = 0, LesserOrEqualThan = 1, Equal = 2, greaterOrEqualThan = 3, greaterThan = 4};
    enum dateCompareMode {YoungerThan = 0, YoungerOrExactThan = 1, Exact = 2, OlderOrExtactThan = 3, OlderThan = 4};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    enum FileType {Folder = 0x060, File = 0x061,All = 0x062, unresolved = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}

template<class TModelBuilder>
class IFileListService
{
public:
    virtual QStringList allFiles(const QStringList &paths, const int &filter) = 0;
    virtual DefaultFileModelList fileModelDelegates(const QStringList &paths, const int &filter) = 0;
    virtual DefaultFileModelList filterFileModelsThatMatch(const DefaultFileModelList &fileModelDelegates, QString &str, bool dontMatch = false, bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsThatContain(const DefaultFileModelList &fileModelDelegates, QString &str, bool dontContain = false, bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToSize(const DefaultFileModelList &fileModelDelegates, const int &size, const int &mode = FilesContext::LesserThan) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToDate(const DefaultFileModelList &fileModelDelegates,
                                                          const int &day,
                                                          const int &month,
                                                          const int &year,
                                                          const int &mode = FilesContext::YoungerThan,
                                                          const int &dateMode = FilesContext::DateCreated) = 0;

    virtual TModelBuilder *modelBuilderService() = 0;
    virtual void setModelBuilderService(TModelBuilder *service) = 0;
};


#endif // IFILELISTBUILDER_H
