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

class rulePathSelector : public TextPathEdit
{
public:
    rulePathSelector(QWidget *parent = nullptr);

public slots:


protected:

protected slots:
    void on_toolButton_clicked() override;

private:
    rulePathSelectorDialog*pDialog;
};

#endif // RULEPATHSELECTOR_H
