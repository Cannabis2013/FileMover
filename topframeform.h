#ifndef TOPFRAMEFORM_H
#define TOPFRAMEFORM_H

#include <QWidget>
#include <QMouseEvent>
#include <qlabel.h>
#include <qcursor.h>
#include <qpushbutton.h>
#include <iostream>

using namespace std;

namespace Ui {
    class TopFrameForm;
}

class TopFrameForm : public QWidget
{
    Q_OBJECT

public:
    explicit TopFrameForm(QWidget *parent = nullptr);
    ~TopFrameForm();


    QString FrameTitle() const;
    void setFrameTitle(QString title);

signals:
    void moveParent(QPoint pos);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::TopFrameForm *ui;

    QPoint mOffset;
    QLabel *frameTitle;
    QWidget *topWidget;
    QPushButton *exitButton;
    bool isMousePressed = false;
};

#endif // TOPFRAMEFORM_H
