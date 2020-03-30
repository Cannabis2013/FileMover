#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "defaultfilemodelscontext.h"
#include "imodelbuilder.h"
#include "filescontext.h"

namespace FilesContext
{
    class IFileListService;
}

template<class TModelBuilder>
class IFileListService
{
public:
    virtual QStringList allFiles(const QStringList &paths, const int &filter) = 0;
    virtual DefaultFileModelList buildFileModels(const int &filter, const QStringList &paths = QStringList()) = 0;
    virtual DefaultFileModelList filterFileModelsThatMatch(const DefaultFileModelList &buildFileModels, QString &str, bool dontMatch = false, bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsThatContain(const DefaultFileModelList &buildFileModels, QString &str, bool dontContain = false, bool suffix = false) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToSize(const DefaultFileModelList &buildFileModels, const int &size, const int &mode = FilesContext::LesserThan) = 0;
    virtual DefaultFileModelList filterFileModelsAccordingToDate(const DefaultFileModelList &buildFileModels,
                                                          const int &day,
                                                          const int &month,
                                                          const int &year,
                                                          const int &mode = FilesContext::YoungerThan,
                                                          const int &dateMode = FilesContext::DateCreated) = 0;

    virtual void appendFileLists(const QStringList &filepaths) = 0;
    virtual QStringList fileLists() = 0;

    virtual TModelBuilder *modelBuilderService() = 0;
    virtual IFileListService<TModelBuilder> *setModelBuilderService(TModelBuilder *service) = 0;
};

typedef IFileListService<IModelBuilder<IFileModel<>,QString>> IDefaultFileListService;


#endif // IFILELISTBUILDER_H
