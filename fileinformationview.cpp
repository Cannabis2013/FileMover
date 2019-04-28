#include "fileinformationview.h"


fileInformationView::fileInformationView(QWidget *parent):
    QWidget(parent)
{
    view = new QTextBrowser(this);
    grid = new QGridLayout(this);

    view->setHtml("<body style='background-color:rgb(81,81,81)'>"
                  "</body>");

    grid->addWidget(view);
    grid->setContentsMargins(0,0,0,0);
    view->show();
}

void fileInformationView::setHtml(QString html)
{
    view->setHtml(html);
}

void fileInformationView::clear()
{
    view->setHtml("<body style='background-color:rgb(81,81,81)'>"
                  "</body>");
}
