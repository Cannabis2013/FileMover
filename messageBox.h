#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <qmessagebox.h>

class messageBx
{
public:
    static bool customBox(QWidget *parent,
                          QString title,
                          QString text,
                          QString but1 = "Ok",
                          QString but2 = QString(),
                          QMessageBox::ButtonRole rol1 = QMessageBox::AcceptRole,
                          QMessageBox::ButtonRole rol2 = QMessageBox::NoRole)
    {

        QMessageBox box;
        box.setWindowTitle(title);
        box.setText(text);

        QString styleSheet = parent->styleSheet();
        box.setStyleSheet(styleSheet);

        QRect pGeo = parent->geometry();
        QRect geo;

        int nH = pGeo.height() /2,
            nW = pGeo.width() /2,
            nX = pGeo.x(),
            nY = pGeo.y();

        box.move(nX + nW,nY + nH);

        QPushButton *b1 = box.addButton(but1,rol1);

        QPushButton *b2 = nullptr;
        if(but2 != QString())
            b2 = box.addButton(but2,rol2);

        box.exec();


        if(reinterpret_cast<QPushButton*>(box.clickedButton()) == b1)
        {
            if(rol1 == QMessageBox::AcceptRole)
                return true;
            else if(rol1 == QMessageBox::NoRole)
                return false;
            else
                return true;

        }
        else if(reinterpret_cast<QPushButton*>(box.clickedButton()) == b2)
        {
            if(rol2 == QMessageBox::AcceptRole)
                return true;
            else if(rol2 == QMessageBox::NoRole)
                return false;
            else
                return false;
        }
        else
        {
            return false;
        }
    }

};

#endif // MESSAGEBOX_H
