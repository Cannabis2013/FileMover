#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "filemodeldelegate.h"

namespace FilesContext
{
    enum sizeComparemode {LesserThan = 0, LesserOrEqualThan = 1, Equal = 2, greaterOrEqualThan = 3, greaterThan = 4};
    enum dateCompareMode {YoungerThan = 0, YoungerOrExactThan = 1, Exact = 2, OlderOrExtactThan = 3, OlderThan = 4};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    enum FileType {Folder = 0x060, File = 0x061,All = 0x062, unresolved = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}

class IFileListService
{
public:
    virtual QStringList allFiles(const QStringList &paths, const int &filter) = 0;
    virtual FileModelList fileModelDelegates(const QStringList &paths, const int &filter) = 0;
    virtual FileModelList filterFileModelsThatMatch(const FileModelList &fileModelDelegates, QString &str, bool dontMatch = false, bool suffix = false) = 0;
    virtual FileModelList filterFileModelsThatContain(const FileModelList &fileModelDelegates, QString &str, bool dontContain = false, bool suffix = false) = 0;
    virtual FileModelList filterFileModelsAccordingToSize(const FileModelList &fileModelDelegates, const int &size, const int &mode = FilesContext::LesserThan) = 0;
    virtual FileModelList filterFileModelsAccordingToDate(const FileModelList &fileModelDelegates,
                                                          const int &day,
                                                          const int &month,
                                                          const int &year,
                                                          const int &mode = FilesContext::YoungerThan,
                                                          const int &dateMode = FilesContext::DateCreated) = 0;
};


#endif // IFILELISTBUILDER_H
