#ifndef SCREENSHOTFRAME_H
#define SCREENSHOTFRAME_H

#include <QGraphicsView>

class screenShotFrame : public QGraphicsView
{
    Q_OBJECT
public:
    explicit screenShotFrame(QWidget *parent = 0);
    //screenShotFrame::~screenShotFrame();

    int moveX;
    int moveY;
    int startX;
    int startY;

    QFrame *selector;

signals:

public slots:


private:
    bool startMove;


protected:
    bool event(QEvent *e);

};

#endif // SCREENSHOTFRAME_H
