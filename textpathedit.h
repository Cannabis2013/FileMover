#ifndef TEXTPATHEDIT_H
#define TEXTPATHEDIT_H

#include <QWidget>
#include <qstring.h>
#include <QModelIndex>
#include <qcompleter.h>
#include <QDirModel>
#include <QVBoxLayout>
#include <iostream>

#include "mywidget.h"

using namespace std;


#include "filepathselectordialog.h"
#include <qtoolbutton.h>

namespace Ui {
class TextPathEdit;
}

// Base class..

class TextPathEdit : public MyWidget
{
    Q_OBJECT

public:
    explicit TextPathEdit(QWidget *parent = nullptr,QString txt = QString());
    ~TextPathEdit();

    void setLineText(QString txt);
    QString text();
    void clear();

signals:

protected:

    void addWidgetToLayout(QWidget *w);
    QVBoxLayout *verticalLayot();
    QLineEdit *line;
    QToolButton *toolButton;
    Ui::TextPathEdit *ui;

protected slots:
    void insertChosenPath(QString p);
    virtual void on_toolButton_clicked();

private:

};

#endif // TEXTPATHEDIT_H
