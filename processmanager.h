#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "ruledefinitions.h"

/*
 * These classes handles the queues.
*/
struct DirectoryObject
{
    long long sz;
    QString dirName;
    QString path;
};
struct ProcessItems
{
    QFileInfoList list;
    rD::fileActionRule ruleMode = rD::none;
    QStringList destinations;
};

class ProcessManager : public QObject
{
    Q_OBJECT

public:
    ~ProcessManager()
    {
        delete this;
    }

    void addToQueue(ProcessItems pI)
    {
        processQeue.append(pI);
        emit wakeUpProcess();
    }

signals:
    void wakeUpProcess();
    void processFinished();


private:
    ProcessItems takeItem()
    {
        if(processQeue.isEmpty())
            return ProcessItems();
        else
            return processQeue.takeFirst();
    }
    bool queueIsEmpty(){return processQeue.isEmpty();}

    QList<ProcessItems>processQeue;
    friend class FileWorker;
};

#endif // PROCESSCONTROLLER_H
