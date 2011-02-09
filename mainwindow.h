#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screenshot.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <QShortcut>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QClipboard>

#include <QtNetwork>
#include <QCryptographicHash>

#include <QxtGlobalShortcut>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool getPenStatusChecked();
    int maxScreenWidth;
    int maxScreenHeight;

    void shootScreen(int xx, int yy, int ww, int hh);
    QString secretkey(QString tmp_datetime);
    QString getCurrentSQLDateTime();

    QFrame* popup1;
    QShortcut *pupupHotKey;

    QString path;
    QString iniHotKeyPrintScreen;
    QString iniHotKeyFastPrintScreen;
    QString iniHotKeyServerLocalSave;
    int iniSaveType;

    QHttp *http;

private slots:

    /* UI Buttons */
    void openButtonAction();
    void saveButtonAction();
    void clearButtonAction();
    void undoButtonAction();
    void cropButtonAction();

    /* UI Group Buttons*/
    void groupButtonAction(bool checked);

    /* SYS TRAY MENU */
    void showAbout();

    /* Process ScreenShots */
    void shootScreenFull();
    void event_startCreateScreenShoting();
    void event_startCreateFullScreenShoting();
    void event_closeScreenShoting();
    void event_startUploadScreenShot();
    void event_sysTrayIconActivated(QSystemTrayIcon::ActivationReason);

    void httpRequestFinished(int, bool);

    void toggleVisibility();


private:
    Ui::MainWindow *ui;

    /* For Process Screenshot */
    QPixmap originalPixmap;

    /* Methods */
    void readConfigApplication();
    void createScreenshotFrame();
    void startUploadScreenShot2();


    /* UI Objects */
    QScrollArea *scrollArea;
    QToolBar *toolBar;
    MyScreenShot *screenshot;


    /* UI Buttons */
    void createButtonEvents();

    QAction *openAction;
    QAction *saveAction;
    QAction *clearAction;
    QAction *undoAction;
    QAction *cropAction;

    QActionGroup *actGroup;

    QAction *penAction;
    QAction *lineAction;
    QAction *arrowAction;
    QAction *vlineAction;
    QAction *hlineAction;
    QAction *textarrowAction;
    QAction *textAction;
    QAction *squareAction;
    QAction *circleAction;

    /* SYS TRAY MENU */
    void createTrayIcon();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;


    QAction *aboutAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *quitAction;
    QAction *processAction;
    QAction *processFullScreenAction;
};

#endif // MAINWINDOW_H
