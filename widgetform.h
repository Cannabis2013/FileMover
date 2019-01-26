#ifndef WIDGETFORM_H
#define WIDGETFORM_H

#include <QWidget>
#include <qframe.h>
#include <qgridlayout.h>
#include <QMouseEvent>
#include <QCloseEvent>
#include <qpoint.h>
#include <qsizepolicy.h>

#include "ui_widgetform.h"


namespace Ui {
    class WidgetForm;
}

class WidgetForm : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetForm(QWidget *parent = nullptr);
    ~WidgetForm();
    void setWidget(QWidget *w, QString title = "Frametitle");

    void setFrameTitle(QString title);
    QString FrameTitle();

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
};

#endif // WIDGETFORM_H
