#ifndef IFILELISTBUILDER_H
#define IFILELISTBUILDER_H

#include <qstringlist.h>
#include "defaultfilemodelscontext.h"
#include "imodelbuilder.h"
#include "filescontext.h"

namespace FilesContext
{

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
