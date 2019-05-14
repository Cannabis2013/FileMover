#ifndef WIDGETFORM_H
#define WIDGETFORM_H

#include <QWidget>
#include <qframe.h>
#include <qgridlayout.h>
#include <QMouseEvent>
#include <QCloseEvent>
#include <qpoint.h>
#include <qsizepolicy.h>
#include <iostream>
#include <memory>
#include <qpointer.h>

#include "ui_customdialog.h"

#include "abstractframeimplementable.h"

using namespace std;

namespace Ui {
    class CustomDialog;
}

class CustomDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDialog(AbstractFrameImplementable *implementable, bool applicationModal = true, QWidget *parent = nullptr);
    ~CustomDialog();

    void setFrameTitle(QString title);
    QString FrameTitle();

    void setWidget(AbstractFrameImplementable *implementable, QString title = "Frametitle");

protected:
    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:

    void on_exitButton_clicked();

private slots:
    void moveGlobalEvent(QPoint pos);

private:

    /*
     * Private methods declarations
     */

    Ui::CustomDialog *ui;
    QFrame *widgetFrame;

    TopFrameForm *topFrame;
    QPointer<AbstractFrameImplementable> mainWidget;
    QGridLayout *grid;
};

#endif // WIDGETFORM_H
