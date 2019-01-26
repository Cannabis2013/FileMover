#ifndef WIDGETFORM_H
#define WIDGETFORM_H

#include <QWidget>
#include <qframe.h>
#include <qgridlayout.h>
#include <QMouseEvent>
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
    void setWidget(QWidget *w);

private slots:

    void on_exitButton_clicked();

protected:
    void closeEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:

    /*
     * Private methods declarations
     */

    void setSizeWidgetSizeAccordingToPolicy(QSizePolicy &policy, QWidget *w);


    Ui::WidgetForm *ui;
    QFrame *widgetFrame;

    QPoint mOffset;

    QWidget *mainWidget;

};

#endif // WIDGETFORM_H
