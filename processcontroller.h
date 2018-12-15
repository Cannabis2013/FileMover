#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformation.h"
#include "ruledefinitions.h"

/*
 * These classes handles the queues.
*/
struct fileObject
{
    long long sz;
    QString path;
};
struct processItems
{
    QFileInfoList list;
    rD::fileActionRule ruleMode = rD::none;
    QStringList destinations;
};

class processController : public QObject
{
    Q_OBJECT
public slots:
    void addToQueue(processItems pI);

signals:
    void wakeUpProcess();
    void processFinished();

protected:
    virtual ~processController();

private:
    void insertItem(processItems pI)
    {
        processQeue.append(pI);
        emit wakeUpProcess();
    }
    processItems takeItem()
    {
        if(processQeue.isEmpty())
            return processItems();
        else
            return processQeue.takeFirst();
    }
    bool queueIsEmpty(){return processQeue.isEmpty();}

    QList<processItems>processQeue;
    friend class fileWorker;
};

#endif // PROCESSCONTROLLER_H
