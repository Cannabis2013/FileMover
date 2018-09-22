#include "rulepathselector.h"

rulePathSelector::rulePathSelector(QWidget *parent):
    textPathEdit(parent)
{
    pDialog = new rulePathDialogSelector(this);

    layout()->addWidget(pDialog);
    connect(pDialog,&rulePathDialogSelector::chosenPath,this,&rulePathSelector::insertChosenPath);
    pDialog->hide();
}

void rulePathSelector::on_toolButton_clicked()
{
    if(pDialog->isHidden())
        pDialog->show();
    else
        pDialog->hide();
}
