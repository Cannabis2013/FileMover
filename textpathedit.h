#ifndef TEXTPATHEDIT_H
#define TEXTPATHEDIT_H

#include <QWidget>
#include <qstring.h>
#include <QModelIndex>
#include <qcompleter.h>
#include <QDirModel>
#include <QVBoxLayout>


#include "filepathselectordialog.h"
#include <qtoolbutton.h>

namespace Ui {
class textPathEdit;
}

// Base class..

class textPathEdit : public QWidget
{
    Q_OBJECT

public:
    explicit textPathEdit(QWidget *parent = nullptr,QString txt = QString());
    ~textPathEdit();

    void setLineText(QString txt);
    QString text();
    void clear();

signals:

protected:
    void addWidgetToLayout(QWidget *w);
    QVBoxLayout *verticalLayot();
    QLineEdit *line;
    QToolButton *toolButton;

protected slots:
    void insertChosenPath(QString p);
    virtual void on_toolButton_clicked();



private:
    Ui::textPathEdit *ui;
};

#endif // TEXTPATHEDIT_H
