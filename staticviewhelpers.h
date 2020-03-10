#ifndef STATICVIEWHELPERS_H
#define STATICVIEWHELPERS_H

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdiriterator.h>
#include <staticfilehelperoperations.h>
#include <QTreeWidgetItem>
#include <qpair.h>

namespace FilesContext
{
    class StaticItemViewHelper
    {
    public:
        static QStringList createHeader(QFileInfo fi = QFileInfo())
        {
            // Filename/dirname - Filepath - file extension - file type - Filesize - lastModified - Last read
            QStringList headers;
            int mp = 1024;


            if(fi.isDir())
            {
                double sz = 0;
                QDirIterator i(fi.filePath(),QDirIterator::Subdirectories);
                while(i.hasNext())
                    sz += QFile(i.next()).size();

                headers << SBC::directoryName(fi.absoluteFilePath())
                        << fi.absoluteFilePath()
                        << ""
                        << "Mappe";

                if(sz <=mp)
                    headers << QString::number(fi.size()) + " b";
                else if(sz > mp && sz <mp*mp)
                    headers << QString::number(sz/mp) + " kb";
                else if(sz >= (mp*mp) && sz <(mp*mp*mp))
                    headers << QString::number(sz/(mp*mp)) + " mb";
                else
                    headers << QString::number(sz/(mp*mp*mp)) + " gb";

                headers << fi.lastModified().toString()
                        << fi.lastRead().toString();
            }
            else if(fi.isFile())
            {
                headers << fi.fileName()
                        << fi.absoluteFilePath()
                        << fi.suffix()
                        << "Fil";

                if(fi.size() <=mp)
                    headers << QString::number(fi.size()) + " b";
                else if(fi.size() > mp && fi.size() <mp*mp)
                    headers << QString::number(fi.size()/mp) + " kb";
                else if(fi.size() >= (mp*mp) && fi.size() <(mp*mp*mp))
                    headers << QString::number(fi.size()/(mp*mp)) + " mb";
                else
                    headers << QString::number(fi.size()/(mp*mp*mp)) + " gb";

                headers << fi.lastModified().toString()
                        << fi.lastRead().toString();
            }
            else
            {

                headers << "Filename"
                        << "Filepath"
                        << "Suffix"
                        << "Type"
                        << "Size"
                        << "Last modified"
                        << "Last read";
            }
            return headers;
        }

        static QTreeWidgetItem* assembleItemModelsFromPath(const QString &path)
        {
            if(!QFileInfo(path).isDir() && !QFileInfo(path).exists())
                return nullptr;

            QTreeWidgetItem *result = new QTreeWidgetItem(createHeader(path));
            QDirIterator ite(path,QDir::NoDotAndDotDot | QDir::AllEntries);
            while(ite.hasNext())
            {
                QFileInfo fInfo = ite.next();

                if(fInfo.isDir())
                {
                    QTreeWidgetItem *dirItem = assembleItemModelsFromPath(fInfo.absoluteFilePath());
                    result->addChild(dirItem);
                }
                else if(fInfo.isFile())
                {
                    result->addChild(new QTreeWidgetItem(createHeader(fInfo)));
                }
            }
            return result;
        }

        static QList<QPair<QString,int>> getListOfSuffixOccuriencies(const QString &p)
        {
            QList<QPair<QString, int> > resultingList;
            QStringList allSufs,tempSufs;
            QDirIterator ite(p,
                             QDir::NoDotAndDotDot |
                             QDir::Files |
                             QDir::Hidden |
                             QDir::System,
                             QDirIterator::Subdirectories);
            while(ite.hasNext())
            {
                QFileInfo i = ite.next();
                if(i.isFile())
                {
                    if(i.suffix() == "")
                        allSufs << "Unknown";
                    else
                        allSufs << i.suffix();
                }
            }

            tempSufs = allSufs;
            for (auto suf : allSufs)
            {
                int taeller = 0;
                if(tempSufs.contains(suf))
                {
                    taeller = tempSufs.count(suf);
                    tempSufs.removeAll(suf);
                    QPair<QString,int> sPar;
                    sPar.first = suf;
                    sPar.second = taeller;
                    resultingList.append(sPar);
                }
            }
            return resultingList;
        }
    };
}

typedef FilesContext::StaticItemViewHelper ByteCollection;

#endif // STATICVIEWHELPERS_H
