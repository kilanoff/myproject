#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QMessageBox>
#include <QLabel>

class MyScreenShot : public QLabel
{
    Q_OBJECT
public:
    explicit MyScreenShot(QWidget *parent = 0);

signals:
public slots:
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
   // MainWindow *myapp;
};

#endif // SCREENSHOT_H

