#ifndef ADDFOLDERWIDGET_H
#define ADDFOLDERWIDGET_H

#include <QWidget>
#include <qdesktopwidget.h>
#include "pathline.h"
#include "addfiledialog.h"

#include <iostream>

using namespace std;

namespace Ui {
class addFolderWidget;
}

class addFolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit addFolderWidget(QWidget *parent = nullptr);
    ~addFolderWidget();

signals:
    void sendPath(QString path);

private slots:
    void sizeAdjust(QSizePolicy::Policy p,int H);
    void on_Add_Button_Clicked();

private:
    Ui::addFolderWidget *ui;
    addFileDialog *fileWidget;
    pathLine *pathWidget;

};

#endif // ADDFOLDERWIDGET_H
