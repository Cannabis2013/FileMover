#include "rulepathselector.h"

rulePathSelector::rulePathSelector(QWidget *parent):
    TextPathEdit(parent)
{
    pDialog = new filepathDialogWidget(Qt::ApplicationModal);
    pDialog->setWindowFlag(Qt::FramelessWindowHint);

    connect(pDialog,&filepathDialogWidget::chosenPath,this,&rulePathSelector::insertChosenPath);
    pDialog->hide();
    setFocusPolicy(Qt::StrongFocus);
    parentWidget()->installEventFilter(this);
    setObjectName("rulePathSelector");
}
void rulePathSelector::on_toolButton_clicked()
{
    if(pDialog->isHidden())
    {
        pDialog->show();
        pDialog->setFocus();
    }
    else
        pDialog->hide();

    QPoint parentGeometry = parentWidget()->geometry().topLeft();
    QPoint newPos = mapToGlobal(parentGeometry);

    QRect rect;
    rect.setX(newPos.x());
    rect.setY(newPos.y() - 2*height() - 5);
    rect.setWidth(this->width());
    rect.setHeight(pDialog->height());

    pDialog->setGeometry(rect);
}
