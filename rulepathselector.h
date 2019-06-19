#ifndef RULEPATHSELECTOR_H
#define RULEPATHSELECTOR_H

#include <QWidget>
#include "textpathedit.h"
#include <iostream>
#include <qmenu.h>
#include <QEvent>
#include <QFocusEvent>

using namespace std;

class rulePathSelector : public TextPathEdit
{
public:
    rulePathSelector(QWidget *parent = nullptr);

protected slots:
    void on_toolButton_clicked() override;

private:
    filepathDialogWidget*pDialog;
};

#endif // RULEPATHSELECTOR_H
