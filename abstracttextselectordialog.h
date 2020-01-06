#ifndef TEXTPATHEDIT_H
#define TEXTPATHEDIT_H

#include <QWidget>
#include <qstring.h>
#include <QModelIndex>
#include <qcompleter.h>
#include <QDirModel>
#include <QVBoxLayout>
#include <iostream>


using namespace std;


#include "filepathdialogwidget.h"
#include <qtoolbutton.h>

#include "imutableobject.h"

namespace Ui {
class AbstractTextSelectorDialog;
}

// Base class..

class AbstractTextSelectorDialog :
        public QWidget,
        public IMutableObject
{
    Q_OBJECT

public:
    explicit AbstractTextSelectorDialog(QWidget *parent = nullptr,QString txt = QString());
    ~AbstractTextSelectorDialog();
    QString text();
    void clear();

public slots:
    void setCurrentFilePath(const QString &fp);


signals:
    void sendCurrentFilePath(QString txt);
    void stateChanged();

protected slots:
    void insertChosenPath(QString p);
    virtual void on_toolButton_clicked()=0;

private:
    QLineEdit *pathSelector;
    Ui::AbstractTextSelectorDialog *ui;

public:
};

#endif // TEXTPATHEDIT_H
