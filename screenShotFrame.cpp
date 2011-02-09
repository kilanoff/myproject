#include "screenshotframe.h"
#include "mainwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QPen>
#include <QPoint>

screenShotFrame::screenShotFrame(QWidget *parent) :
    QGraphicsView(parent)
{
    startX = 100;
    startY = 100;

    moveX = 300;
    moveY = 300;

    startMove=false;

    selector = new QFrame(this,Qt::Tool | Qt::Window | Qt::FramelessWindowHint);
    selector->setStyleSheet("background: black");
    selector->setWindowOpacity(0.3);
    selector->setFrameShadow(Raised);
    selector->hide();
}

bool screenShotFrame::event(QEvent *e)
{

    if (e->type() == QEvent::MouseButtonPress && !startMove)
    {
        startMove = true;

        QMouseEvent *mouse = static_cast<QMouseEvent*>(e);
        startX = mouse->pos().x();
        startY = mouse->pos().y();

        selector->setGeometry(startX, startY, moveX-startX, moveY-startY);
        selector->show();
    }

    if (e->type() == QEvent::MouseMove && startMove)
    {
        QMouseEvent *mouse = static_cast<QMouseEvent*>(e);
        moveX = mouse->pos().x();
        moveY = mouse->pos().y();

        selector->setGeometry(startX, startY, moveX-startX, moveY-startY);
    }

    if (e->type() == QEvent::MouseButtonRelease && startMove)
    {
        startMove = false;

        selector->hide();

        reinterpret_cast<MainWindow*>(this->parent())->shootScreen(startX,startY,moveX-startX,moveY-startY);
        close();
    }

    return true;
}

