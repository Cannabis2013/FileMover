#include "rulepathdialogselector.h"

rulePathSelectorDialog::rulePathSelectorDialog():
    filepathSelectorDialog()
{
    this->setStyleSheet("QTreeView{border:border: 0px solid black;background-color:white}"
                        "QPushButton{background-color: rgba(255, 255, 255, 175);"
                        "border:0 px solid black;}"
                        "QPushButton::hover{background-color:white;}");

    setWindowModality(Qt::ApplicationModal);
}




