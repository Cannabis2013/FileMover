#ifndef TESTFILECREATOR_H
#define TESTFILECREATOR_H

#include <qfile.h>
#include <qlist.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <QDateTime>
#include <qmap.h>
#include "mainapplication.h"
#include <QFileInfoList>


extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

#define NUMBER_OF_MAPPINGS 50


struct VIRTUAL_FILE_OBJECT
{
    QString filePath;
    myDateTime dateCreated; // Has to be altered to perform tests
    QFileInfo additionalInformation; // Static information not to be altered

};

const QString dummyContent = "This is a testfile containing only dummy content\n";

class TestFileCreator
{
public:
    TestFileCreator(const QString &filePath,const QStringList &fileNames, const bool &persist);
    bool emptyTestFolder(QString dirPath = QString());

private:
    // Private member methods
    void fillDateMappings();
    void appendVirtualFileObject(const QFileInfo &file);

    // Member variables
    QMap<int,myDateTime> dateMappings;
    QList<VIRTUAL_FILE_OBJECT> f_objects;
    const QString currentWorkingPath;
    const bool isPersisted;
};

#endif // TESTFILECREATOR_H
