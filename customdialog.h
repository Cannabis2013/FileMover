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

#include "frameimplementable.h"

using namespace std;

namespace Ui {
    class CustomDialog;
}

class CustomDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDialog(FrameImplementable *implementable, bool applicationModal = true, QWidget *parent = nullptr);
    ~CustomDialog();

    void setFrameTitle(QString title);
    QString FrameTitle();

    void setWidget(FrameImplementable *implementable, QString title = "Frametitle");

protected:
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
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
    bool isMouseButtonPressed = false;
    QFrame *widgetFrame;
    int margin;
    TopFrameForm *topFrame;
    QPointer<FrameImplementable> mainWidget;
    QGridLayout *grid;

    QPoint mousePressPosition;
    QRect tempGeometry;
};

#endif // WIDGETFORM_H
