#ifndef STATICFILEHELPEROPERATIONS_H
#define STATICFILEHELPEROPERATIONS_H

#include <QString>
#include <math.h>
#include <StaticStringsCollection.h>

namespace FilesContext {

    class StaticByteCollection
    {
    public:
        static double convertFromBytes(quint64 numb, QString &denote, int dec = 0)
        {
            quint16 d = 1,
                    dc = 10,
                    sizeKb = 1024,
                    sizeMb = sizeKb *1024,
                    sizeGb = sizeMb * 1024;

            double byteSize = 1024, byteX, result;

            if(dec != 0)
            {
                for(int a = 0;a <dec;a++)
                {
                    d *= dc;
                }
            }
            if(numb <sizeKb)
            {
                byteX = numb;
                denote = "bytes";
            }
            else if(numb > sizeKb && numb <sizeMb)
            {
                byteX = numb / byteSize;
                denote = "kb";
            }
            else if(numb > sizeMb && numb < sizeGb)
            {
                byteX = (numb /byteSize) / byteSize;
                denote = "mb";
            }
            else
            {
                byteX = ((numb / byteSize) / byteSize) / byteSize;
                denote = "gb";
            }
            return result = round(byteX *d) / d;
        }

        static qint64 convertToBytes(const uint &to, const QString &from)
        {
            if(from == "b")
                return to;
            else if(from == "kb")
                return to*1024;
            else if(from == "mb")
                return to*1024*1024;
            else if(from == "gb")
                return to*1024*1024*1024;
            else
                return to;
        }

        static QString directoryName(const QString &path)
        {
            QString p = path;
            QString result;

            if(StaticStringCollections::hasTrailingBackslash(path))
                p.remove(path.length() -1,1);

            for (int i = p.length()-1; i >= 0; --i) {
                QChar c = p.at(i);
                if(c == '/' || c == '\\')
                    i = -1;
                else {
                    result.prepend(c);
                }
            }
            return result;
        }
    };
}


typedef FilesContext::StaticByteCollection SBC;


#endif // STATICFILEHELPEROPERATIONS_H
