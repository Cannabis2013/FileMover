#ifndef TOPFRAMEFORM_H
#define TOPFRAMEFORM_H

#include <QWidget>
#include <QMouseEvent>
#include <qlabel.h>

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
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::TopFrameForm *ui;

    QPoint mOffset;
    QLabel *frameTitle;
};

#endif // TOPFRAMEFORM_H
