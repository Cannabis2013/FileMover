#include "mytreeview.h"

myTreeView::myTreeView(QWidget *parent):
    QTreeView (parent)
{
    setFocusPolicy(Qt::StrongFocus);
}
