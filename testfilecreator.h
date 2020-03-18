#ifndef TESTFILECREATOR_H
#define TESTFILECREATOR_H

#include <qfile.h>
#include <qlist.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <QDateTime>
#include <qmap.h>
#include "applicationdomain.h"
#include <QFileInfoList>
#include <iterator>


#ifdef __MINGW64__
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

#define NUMBER_OF_DATE_MAPPINGS 50
#define MIN_YEAR 2000


struct VIRTUAL_FILE_OBJECT
{
    QString filePath;
    QDateTime dateCreated; // Has to be altered to perform tests
    QFileInfo additionalInformation; // Static information not to be altered
    QString fileName() const {return additionalInformation != QFileInfo() ?
                    additionalInformation.fileName() :
                    QString();}
};

class Virtual_Objects
{
public:
    explicit Virtual_Objects();
    VIRTUAL_FILE_OBJECT getVirtualObjectFromFilePath(const QString &path) const;
    VIRTUAL_FILE_OBJECT getVirtualObjectFromIndex(const int &index) const;
    VIRTUAL_FILE_OBJECT getVirtualObjectFromFileName(const QString &name);
    const Virtual_Objects &operator<<(const VIRTUAL_FILE_OBJECT &obj);
    bool operator==(Virtual_Objects objects) const;
    bool operator!=(Virtual_Objects objects) const;
    VIRTUAL_FILE_OBJECT operator[](long a);

    inline int count() const{return _objects.count();}

private:
    QList<VIRTUAL_FILE_OBJECT> _objects;
};

#define DUMMY_CONTENT "This is a testfile containing only dummy content\n"

class TestFileCreator
{
public:
    TestFileCreator();

    Virtual_Objects *createFiles(const QString &directory, const QStringList &fileNames);
    VIRTUAL_FILE_OBJECT VirtualObject(const QString &fileName);
    Virtual_Objects VirtualObjects(const QString &filePath);
    bool emptyTestFolder(const QString &dirPath);

    Virtual_Objects virtualObjects() const;

private:
    // Private member methods
    void fillDateMappings();
    void appendVirtualFileObject(const QFileInfo &file);
    // Member variables
    QMap<int,QDateTime> dateMappings;
    Virtual_Objects _virtualObjects;
};

#endif // TESTFILECREATOR_H
