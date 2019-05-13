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

#include "ui_widgetform.h"

using namespace std;

namespace Ui {
    class WidgetForm;
}

class WidgetForm : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetForm(QWidget *parent = nullptr, bool applicationModal = true);
    ~WidgetForm();

    void setFrameTitle(QString title);
    QString FrameTitle();

    void Show();

protected:
    void setWidget(QWidget *w, QString title = "Frametitle");

private slots:

    void on_exitButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void moveGlobalEvent(QPoint pos);

private:

    /*
     * Private methods declarations
     */

    void adjustSizeAccordingToWidgetPolicy();

    Ui::WidgetForm *ui;
    QFrame *widgetFrame;

    TopFrameForm *topFrame;
    QWidget *mainWidget;
    QGridLayout *grid;
};

#endif // WIDGETFORM_H
