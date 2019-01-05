#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <qcombobox.h>
#include <QModelIndex>
#include <QListView>
#include <iostream>

using namespace std;

class myComboBox : public QComboBox
{

    Q_OBJECT
public:
    myComboBox(int parentRowIndex)
    {
        rowIndex = parentRowIndex;
        connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(ListChanged(int)));
    }
    QStringList items() const
    {
        QStringList allItems;
        for (int i = 0; i < count(); ++i) {
            allItems << itemText(i);
        }
        return allItems;
    }
    int indexOf(const QString str)
    {
        QStringList allItems = items();
        for (int i = 0; i < allItems.count(); ++i) {
            if(allItems.at(i) == str)
                return i;
        }
        return -1;
    }
signals:
    void currentListChanged(int index,int indx);
private slots:
    void ListChanged(const int i)
    {
        emit currentListChanged(i,rowIndex);
    }
private:
    int rowIndex;
};
#endif // MYCOMBOBOX_H
