#include "rulepathselector.h"

rulePathSelector::rulePathSelector(QWidget *parent):
    textPathEdit(parent)
{
    pDialog = new rulePathSelectorDialog(this);

    layout()->addWidget(pDialog);
    connect(pDialog,&rulePathSelectorDialog::chosenPath,this,&rulePathSelector::insertChosenPath);
    pDialog->hide();
}

void rulePathSelector::on_toolButton_clicked()
{
    if(pDialog->isHidden())
        pDialog->show();
    else
        pDialog->hide();
}
