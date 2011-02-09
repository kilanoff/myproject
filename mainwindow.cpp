#include "mainwindow.h"
#include "screenshot.h"
#include "ui_mainwindow.h"
#include "screenShotFrame.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* START APP */
    path = QDir::homePath();
    readConfigApplication();

    createTrayIcon();
    createButtonEvents();

    QScrollArea *scrollArea = qFindChild<QScrollArea *>(this,"scrollArea");
    setCentralWidget(scrollArea);

    screenshot = new MyScreenShot(this);
    screenshot->setBackgroundRole(QPalette::Base);
    screenshot->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    screenshot->setScaledContents(true);
    screenshot->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    screenshot->setStyleSheet("background: #000000");
    scrollArea->setWidget(screenshot);
    scrollArea->setStyleSheet("background: #000000");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::toggleVisibility()
{
    QMessageBox::warning(0,"fff","fff");
}

/* EVENT - реакция на хот кей для выбора куска экрана */
void MainWindow::event_startCreateScreenShoting()
    {
    /* Скрываем основное окно */
    hide();

    /* Создаем окно для выбора скриншота */
    createScreenshotFrame();
    }

/* EVENT - создание полного скриншота */
void MainWindow::event_startCreateFullScreenShoting()
{
    shootScreenFull();
}

/* EVENT - закрываем окно созданяи скриншота, т.е. отмена действия  */
void MainWindow::event_closeScreenShoting()
{
    /* Скрываем окно для выбора скриншота */
    popup1->hide();

    /* Глушим хоткей, удаляем его и удаляем попап */
    pupupHotKey->disconnect(popup1);
    delete pupupHotKey;

    /* Показываем основное окно */
    show();
}

/* EVENT - отправка POST Данных */
void MainWindow::event_startUploadScreenShot()
{
    startUploadScreenShot2();
}















QString MainWindow::secretkey(QString tmp_datetime)
{
    QByteArray rbytes = QCryptographicHash::hash(QString(tmp_datetime+"lapizdos66ru").toAscii(), QCryptographicHash::Md5).toHex();

    return QString::fromAscii(rbytes).toUpper();
}

QString MainWindow::getCurrentSQLDateTime()
{
    QDateTime now = QDateTime::currentDateTime();

    return now.toString("yyyy-MM-dd H:mm:ss");
}


void MainWindow::startUploadScreenShot2()
{
    QString tmp_datetime;
    QString secret_key;

    tmp_datetime = getCurrentSQLDateTime();
    secret_key = secretkey(tmp_datetime);

    QByteArray bytes2;
    QBuffer buffer(&bytes2);
    buffer.open(QIODevice::WriteOnly);
    screenshot->pixmap()->save(&buffer, "JPG");


    http = new QHttp(this);
    connect(http, SIGNAL(requestFinished(int,bool)), SLOT(httpRequestFinished(int, bool)));

    QString boundary = "---------------------------723690991551375881941828858";


    QByteArray data;

    // key
    data += QString("--" + boundary + "\r\n").toAscii();
    data += "Content-Disposition: form-data; name=\"key\"\r\n\r\n";
    data += secret_key+"\r\n"; // put password if needed

    // datetime
    data += QString("--" + boundary + "\r\n").toAscii();
    data += "Content-Disposition: form-data; name=\"datetime\"\r\n\r\n";
    data += tmp_datetime+"\r\n";

    // qt
    data += QString("--" + boundary + "\r\n").toAscii();
    data += "Content-Disposition: form-data; name=\"qt\"\r\n\r\n";
    data += "yes\r\n";

    // file
    data += QString("--" + boundary + "\r\n").toAscii();
    data += "Content-Disposition: form-data; name=\"uploadfile\"; filename=\"pic2net.qt.jpg\"\r\n";
    data += "Content-Transfer-Encoding: binary\r\n";
    data += "Content-Type: image/jpeg\r\n\r\n";
    data += bytes2;
    data += "\r\n";

    data += QString("--" + boundary + "--\r\n").toAscii();


    QHttpRequestHeader header("POST", "/p2n_actions.php");
    header.setValue("Accept", "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
    header.setValue("Accept-Charset", "windows-1251,utf-8;q=0.7,*;q=0.3");
    header.setValue("Accept-Encoding", "gzip,deflate,sdch");
    header.setValue("Accept-Language", "ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4");
    header.setValue("Cache-Control","max-age=0");
    header.setValue("Connection", "keep-alive");
    header.setValue("Content-Length", QString::number(data.length()));
    header.setValue("Content-Type", "multipart/form-data; boundary=" + boundary);
    header.setValue("Host", "pic2net.ru");
    header.setValue("Origin", "http://pic2net.ru");
    header.setValue("Referer", "http://pic2net.ru");
    header.setValue("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.10 (KHTML, like Gecko) Chrome/8.0.552.237 Safari/534.10");

    http->setHost("pic2net.ru");
    http->request(header, data);
}

void MainWindow::httpRequestFinished(int, bool)
{
    QHttpResponseHeader responce = http->lastResponse();

    qDebug() << responce.statusCode() << responce.toString();
    if (responce.statusCode()==200)
    {
        QByteArray otv = http->readAll();

        trayIcon->showMessage(
                tr("Ссылка на скриншот скопирована в буфер обмена"),
                "URL: "+otv,
                QSystemTrayIcon::Information,
                5 * 1000
                );

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(otv);
    }
}




/* создаем окно, для выбора части экрана для создания скриншота */
void MainWindow::createScreenshotFrame()
{
    /* Создание окна выбора */
    popup1 = new screenShotFrame(this);
    popup1->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool | Qt::Window | Qt::FramelessWindowHint);
    popup1->setGeometry(0,0,maxScreenWidth, maxScreenHeight);
    popup1->setWindowOpacity(0.2);
    popup1->setMouseTracking(true);
    popup1->show();

    /* Хоткей на полный принт скрин */
    pupupHotKey = new QShortcut(QKeySequence("Esc"), popup1);
    pupupHotKey->setContext(Qt::ApplicationShortcut);
    pupupHotKey->connect(pupupHotKey, SIGNAL(activated()), this, SLOT(event_closeScreenShoting()));

    /* Делаем активным окно выбора области */
    //popup1->topLevelWidget();
    popup1->activateWindow();
    popup1->show();
}

/* Делает полный снимок экрана */
void MainWindow::shootScreenFull()
{
    shootScreen(0,0,-1,-1);
}

/* получаем скрин зоны экрана */
void MainWindow::shootScreen(int xx, int yy, int ww, int hh)
{
    /*
        TODO:
        тут пока выбирается и сумируется ширина всех окон,
        но это не правильно, мониторы могут быть и вертикально расположены
        в этом случае надо суммировать вертикальную составляющую
    */

    if (ww==-1)
        {
        ww = maxScreenWidth;
        }

    screenshot->setGeometry(xx,yy,ww,hh);

    originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), xx, yy, ww, hh);

    screenshot->setPixmap(originalPixmap);
    screenshot->adjustSize();
    screenshot->update();

    show();
}



















bool MainWindow::getPenStatusChecked()
{
    return penAction->isChecked();
}

void MainWindow::createButtonEvents()
{
    openAction = new QAction(QIcon(":/res/open.jpg"), tr("Открыть изображение"), this);
    connect(openAction, SIGNAL(triggered()),this,SLOT(openButtonAction()));

    saveAction = new QAction(QIcon(":/res/save.jpg"), tr("Сохранить изображение"), this);
    connect(saveAction, SIGNAL(triggered()),this,SLOT(saveButtonAction()));

    clearAction = new QAction(QIcon(":/res/clear.jpg"), tr("Очистить"), this);
    connect(clearAction, SIGNAL(triggered()),this,SLOT(clearButtonAction()));

    undoAction = new QAction(QIcon(":/res/undo.jpg"), tr("Отменить"), this);
    connect(undoAction, SIGNAL(triggered()),this,SLOT(undoButtonAction()));

    cropAction = new QAction(QIcon(":/res/crop.jpg"), tr("Кадрировать"), this);
    connect(cropAction, SIGNAL(triggered()),this,SLOT(cropButtonAction()));



    actGroup = new QActionGroup(this);

    penAction = new QAction(QIcon(":/res/pen.jpg"), tr("Карандаш"), actGroup);
    connect(penAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    penAction->setCheckable(true);

    lineAction = new QAction(QIcon(":/res/line.jpg"), tr("Линия"), actGroup);
    connect(lineAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    lineAction->setCheckable(true);

    arrowAction = new QAction(QIcon(":/res/arrow.jpg"), tr("Стрелка"), actGroup);
    connect(arrowAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    arrowAction->setCheckable(true);

    vlineAction = new QAction(QIcon(":/res/line_vert.jpg"), tr("Вертикальная линия"), actGroup);
    connect(vlineAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    vlineAction->setCheckable(true);

    hlineAction = new QAction(QIcon(":/res/line_horz.jpg"), tr("Горизонтальная линия"), actGroup);
    connect(hlineAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    hlineAction->setCheckable(true);

    textarrowAction = new QAction(QIcon(":/res/arr_t.jpg"), tr("Стрелка с текстом"), actGroup);
    connect(textarrowAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    textarrowAction->setCheckable(true);

    textAction = new QAction(QIcon(":/res/txt.jpg"), tr("Вставка текста"), actGroup);
    connect(textAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    textAction->setCheckable(true);

    squareAction = new QAction(QIcon(":/res/rect.jpg"), tr("Рамка"), actGroup);
    connect(squareAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    squareAction->setCheckable(true);

    circleAction = new QAction(QIcon(":/res/circ.jpg"), tr("Окружность"), actGroup);
    connect(circleAction, SIGNAL(toggled(bool)),this,SLOT(groupButtonAction(bool)));
    circleAction->setCheckable(true);



    QToolBar *toolBar = qFindChild<QToolBar *>(this, "toolBar");
    toolBar->setMovable(false);

    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(clearAction);
    toolBar->addAction(undoAction);
    toolBar->addAction(cropAction);

    /* Группы кнопок */
    toolBar->addAction(penAction);
    toolBar->addAction(lineAction);
    toolBar->addAction(arrowAction);

    toolBar->addAction(vlineAction);
    toolBar->addAction(hlineAction);
    toolBar->addAction(textarrowAction);
    toolBar->addAction(textAction);
    toolBar->addAction(squareAction);
    toolBar->addAction(circleAction);
}





void MainWindow::openButtonAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QDir::currentPath());

    if (!fileName.isEmpty())
        {
        QImage image(fileName);
        if (image.isNull())
            {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
            return;
            }

        screenshot->setPixmap(QPixmap::fromImage(image));
        screenshot->adjustSize();
        screenshot->update();
        }
}
void MainWindow::saveButtonAction()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QDir::currentPath());
    if (!fileName.isEmpty())
    {
        QImage image = screenshot->pixmap()->toImage();


        image.save(fileName);
    }
}
void MainWindow::clearButtonAction()
{
    QImage image;
    screenshot->setPixmap(QPixmap::fromImage(image));
    screenshot->adjustSize();
}
void MainWindow::undoButtonAction()
{
 //   QMessageBox::warning(0,"Warning", "undo button click");


}

void MainWindow::cropButtonAction()
{


}

void MainWindow::groupButtonAction(bool checked)
{

}








void MainWindow::createTrayIcon()
{
    /* TRAY MENU ACTIONs */
    processAction = new QAction(tr("Выделить область ("+iniHotKeyPrintScreen.toAscii()+")"), this);
    processAction->setFont(QFont( "", -1, QFont::Bold ));
    connect(processAction, SIGNAL(triggered()), this, SLOT(event_startCreateScreenShoting()));

    processFullScreenAction = new QAction(tr("Быстрый скриншот ("+iniHotKeyFastPrintScreen.toAscii()+")"), this);
    processFullScreenAction->setFont(QFont( "", 8, QFont::Bold ));
    connect(processFullScreenAction, SIGNAL(triggered()), this, SLOT(event_startCreateFullScreenShoting()));

    aboutAction = new QAction(tr("О программе"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    minimizeAction = new QAction(tr("Свернуть"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Развернуть"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    quitAction = new QAction(tr("Закрыть"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));



    /* TRAY ICON */
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(processAction);
    trayIconMenu->addAction(processFullScreenAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/res/pen.jpg"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(event_sysTrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::event_sysTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        if (isVisible())
        {
            hide();
        }
        else
        {
            show();
        }
    }
}


void MainWindow::showAbout()
{
    QMessageBox::warning(0,"Warning", "Warning message text");
}







void MainWindow::readConfigApplication()
{
    /* ролучаем максимальный размер экрана (сумма ширинок мониторов) */
    maxScreenWidth = 0;
    maxScreenHeight = 0;
    for (int a = 0; a < QApplication::desktop()->screenCount(); a++)
        {
        QRect r;
        r = QApplication::desktop()->screenGeometry(a);

        maxScreenWidth += r.width();
        if (maxScreenHeight==0) maxScreenHeight = r.height();
        }




    QString filename = path+"/pic2net.ini";
    QFileInfo filenameInfo(filename);
    if (!filenameInfo.isFile())
    {
        QFile f1( filename );
        f1.open( QIODevice::WriteOnly );
        f1.write("[options]\n");

        /* Кнопка по умолчанию - сделать скриншот */
        iniHotKeyPrintScreen = "Alt+1";
        f1.write("hotkey=");
        f1.write(iniHotKeyPrintScreen.toAscii());
        f1.write("\n");

        /* Кнопка по умолчанию - сделать быстрый полный скриншот */
        iniHotKeyFastPrintScreen = "Alt+2";
        f1.write("fasthotkey=");
        f1.write(iniHotKeyFastPrintScreen.toAscii());
        f1.write("\n");

        /* Кнопка по умолчанию - Сохранить на сервер или на локал скриншот */
        iniHotKeyServerLocalSave = "Alt+3";
        f1.write("savehotkey=");
        f1.write(iniHotKeyServerLocalSave.toAscii());
        f1.write("\n");

        /* Тип сохранения скриншота (1 - только сервер, 2 - сервер и локальный файл, 3 - только локальный файл) */
        iniSaveType = 1;
        f1.write("savetype=");
        f1.write(QString::number(iniSaveType).toAscii());
        f1.write("\n");

        f1.close();
    }

    /* Открываем файл настроек - читаем */
    QSettings settings(filename, QSettings::IniFormat);

    /* Кнопка быстрого скриншота */
    iniHotKeyPrintScreen = settings.value("options/hotkey","Alt+1").toString();
    iniHotKeyFastPrintScreen = settings.value("options/fasthotkey","Alt+2").toString();
    iniHotKeyServerLocalSave = settings.value("options/savehotkey","Alt+3").toString();
    iniSaveType = settings.value("options/savetype", "r").toInt();




    /* Хоткей на выборочный скриншот */
    QxtGlobalShortcut* shortcut = new QxtGlobalShortcut(this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(event_startCreateScreenShoting()));
    shortcut->setShortcut(QKeySequence(iniHotKeyPrintScreen));

    /* Хоткей на полный принт скрин */
    QxtGlobalShortcut *sh2 = new QxtGlobalShortcut(this);//(QKeySequence(iniHotKeyFastPrintScreen),this);
    connect(sh2, SIGNAL(activated()), this, SLOT(event_startCreateFullScreenShoting()));
    sh2->setShortcut(QKeySequence(iniHotKeyFastPrintScreen));

    /* http send */
    QxtGlobalShortcut *sh3 = new QxtGlobalShortcut(this);//(QKeySequence(iniHotKeyServerLocalSave),this);
    connect(sh3,SIGNAL(activated()),this,SLOT(event_startUploadScreenShot()));
    sh3->setShortcut(QKeySequence(iniHotKeyServerLocalSave));
}
