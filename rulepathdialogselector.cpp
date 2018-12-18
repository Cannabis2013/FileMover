#include "rulepathdialogselector.h"

rulePathSelectorDialog::rulePathSelectorDialog(QWidget *parent):
    filepathSelectorDialog(parent)
{
    this->setStyleSheet("QTreeView{border:border: 0px solid black;background-color:white}"
                        "QPushButton{background-color: rgba(255, 255, 255, 175);"
                        "border:0 px solid black;}"
                        "QPushButton::hover{background-color:white;}");
    setFocusPolicy(Qt::StrongFocus);
    installEventFilter(this);
}

bool rulePathSelectorDialog::eventFilter(QObject *watcher, QEvent *event)
{
    QString name = watcher->objectName();
    if(event->type() == QFocusEvent::FocusIn)
        print(name);
}




