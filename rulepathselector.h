#ifndef RULEPATHSELECTOR_H
#define RULEPATHSELECTOR_H

#include <QWidget>
#include "textpathedit.h"
#include "rulepathdialogselector.h"
#include <iostream>
#include <qmenu.h>
#include <QEvent>
#include <QFocusEvent>

using namespace std;

class rulePathSelector : public textPathEdit
{
public:
    rulePathSelector(QWidget *parent = nullptr);

protected:

protected slots:
    void on_toolButton_clicked() override;

private:
    bool hasAllChildrenFocus();

    rulePathSelectorDialog*pDialog;


};

#endif // RULEPATHSELECTOR_H
