#ifndef TEXTPATHEDIT_H
#define TEXTPATHEDIT_H

#include <QWidget>
#include <qstring.h>
#include <QModelIndex>
#include <qcompleter.h>
#include <QDirModel>
#include <QVBoxLayout>
#include <iostream>

#include "mutuablewidget.h"

using namespace std;


#include "filepathdialogwidget.h"
#include <qtoolbutton.h>

namespace Ui {
class AbstractTextSelectorDialog;
}

// Base class..

class AbstractTextSelectorDialog : public MutuableWidget
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

protected slots:
    void insertChosenPath(QString p);
    virtual void on_toolButton_clicked()=0;

private:
    QLineEdit *pathSelector;
    Ui::AbstractTextSelectorDialog *ui;
};

#endif // TEXTPATHEDIT_H
