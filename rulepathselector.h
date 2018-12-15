#ifndef RULEPATHSELECTOR_H
#define RULEPATHSELECTOR_H

#include <QWidget>
#include "textpathedit.h"
#include "rulepathdialogselector.h"
#include <iostream>
#include <qmenu.h>


using namespace std;

class rulePathSelector : public textPathEdit
{
public:
    rulePathSelector(QWidget *parent = nullptr);

protected slots:
    void on_toolButton_clicked() override;
private:
    rulePathSelectorDialog*pDialog;


};

#endif // RULEPATHSELECTOR_H
