#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <qobject.h>
#include <iostream>

using namespace std;

/*
 * Provides funcionalities to print objects.
 * Provides the 'notify observers' and message features.
 */

class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject(){}

    enum printMode{lineBreak,noLineBreak};
    enum listMode{noBreak,breakLines};

    void print(int val, printMode mode = printMode::lineBreak)
    {
        {
            if(mode == printMode::lineBreak)
                cout << val << endl;
            else
                cout << val;
        }
    }
    void print(QString text, printMode mode = printMode::lineBreak)
    {
        if(mode == printMode::lineBreak)
            cout << text.toStdString() << endl;
        else
            cout << text.toStdString();
    }
    void print(QStringList textList, listMode breakBetweenElemenets = listMode::noBreak ,printMode mode = printMode::lineBreak)
    {
        for(QString str : textList)
        {
            cout << str.toStdString();
            if(breakBetweenElemenets == listMode::breakLines)
                cout << endl;
        }
        if(mode == printMode::lineBreak)
            cout << endl;
    }

    void addEntry(const QString &entry)
    {
        entries << entry;
    }

    void extractEntries()
    {
        print("###Extracting string entries###");
        for (QString entry : entries)
        {
            print(entry);
        }
        print("###Done extracting###");

        entries.clear();
    }

signals:
    void stateChanged();

    void sendSystemTrayMessage(const QString &title,const QString &msg);
    void sendStatusLineMessage(const QString &msg);

private:
    QStringList entries;

};

#endif // MYOBJECT_H

