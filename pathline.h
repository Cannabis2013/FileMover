#ifndef PATHLINE_H
#define PATHLINE_H

#include <qlineedit.h>
#include "textpathedit.h"

class pathLine : public textPathEdit
{
    Q_OBJECT

public:
    pathLine(QWidget *parent = nullptr);

public slots:
    void setCurrentFilePath(QString fp);

signals:
    void showFileExplorer();
    void sendCurrentFilePath(QString txt);

protected:
    void on_toolButton_clicked() override;

private slots:
};

#endif // PATHLINE_H
