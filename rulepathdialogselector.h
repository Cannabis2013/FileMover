#ifndef RULEPATHDIALOGSELECTOR_H
#define RULEPATHDIALOGSELECTOR_H


#include <QFocusEvent>
#include <qevent.h>

#include "filepathselectordialog.h"

class rulePathSelectorDialog : public filepathSelectorDialog
{
public:
    rulePathSelectorDialog(QWidget *parent= nullptr);

protected:
    virtual bool eventFilter(QObject *watcher,QEvent *event) override;

};

#endif // RULEPATHDIALOGSELECTOR_H
