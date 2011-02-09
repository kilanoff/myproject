#include "screenshot.h"
#include "mainwindow.h"



MyScreenShot::MyScreenShot(QWidget *parent) :
    QLabel(parent)

{

}

void MyScreenShot::paintEvent(QPaintEvent *e)
{
   QLabel::paintEvent(e);
}

void MyScreenShot::mouseMoveEvent(QMouseEvent *ev)
{

}

void MyScreenShot::mousePressEvent(QMouseEvent *ev)
{
    if (reinterpret_cast<MainWindow*>(this->parent())->getPenStatusChecked())
    {
        QMessageBox::warning(0,"Warning", "pen is checked");
    }
    else
    {
        QMessageBox::warning(0,"Warning", "pen not checked");
    }
}

void MyScreenShot::mouseReleaseEvent(QMouseEvent *ev)
{
    QMessageBox::warning(0,"Warning", "mouseReleaseEvent");
}
