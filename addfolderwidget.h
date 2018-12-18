#ifndef ADDFOLDERWIDGET_H
#define ADDFOLDERWIDGET_H

#include <QDialog>
#include <qdesktopwidget.h>
#include "pathline.h"
#include "addfiledialog.h"

#include <iostream>

using namespace std;

namespace Ui {
class addFolderWidget;
}

class addFolderWidget : public QDialog
{
    Q_OBJECT

public:
    explicit addFolderWidget();
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
    QVBoxLayout *vLayout;
    int tempHeight;
};

#endif // ADDFOLDERWIDGET_H
