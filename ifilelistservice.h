#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "filemodeldelegate.h"

class IFileListService
{
public:
    virtual QStringList allFiles(const QStringList &paths, const int &filter,const int &iteratorMode) = 0;
    virtual FileModelList fileModelDelegates(const QStringList &paths, const int &filter) = 0;
    virtual QStringList filterFilesThatMatch(const QStringList &filePaths,QString &str, bool dontMatch = false, bool suffix = false) = 0;
    virtual QStringList filterFilesThatContain(const QStringList &filePaths,QString &str, bool dontContain = false, bool suffix = false) = 0;
    virtual QStringList filterFilesAccordingToSize(const QStringList &filePaths, int &size,int mode = 0) = 0;
    virtual QStringList filterFilesAccordingToDate(const QStringList &filePaths, int day, int month, int year, const int &mode = 0, const int &dateMode = 0) = 0;
};

namespace FilesContext
{
    enum sizeComparemode {LesserThan = 0, LesserOrEqualThan = 1, Equal = 2, greaterOrEqualThan = 3, greaterThan = 4};
    enum dateCompareMode {YoungerThan = 0, YoungerOrExactThan = 1, Exact = 2, OlderOrExtactThan = 3, OlderThan = 4};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    enum FileType {Folder = 0x060, File = 0x061,All = 0x062, unresolved = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}

#endif // IFILELISTBUILDER_H
