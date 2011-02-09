#include <QtGui>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    /* «агружаем ресурс с картинками */
    Q_INIT_RESOURCE(images);

    /* ”станавливаем кодировку приложени€ */
    QTextCodec *cp1251 = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForTr(cp1251);

    /* создаем приложение */
    QApplication app(argc, argv);

    /* ѕровер€ем возможность иметь трей иконку */
    if (!QSystemTrayIcon::isSystemTrayAvailable()) 
        {
        QMessageBox::critical(
			0, 
			QObject::tr("Systray"), 
			QObject::tr("I couldn't detect any system tray on this system.")
			);
        return 1;
        }

    /* «апрещаем быстрый выход, только по коду выхода в ручную */
    QApplication::setQuitOnLastWindowClosed(false);

    /* —оздаем окно и показываем его, запускаем приложение */
    MainWindow w;
    w.show();
    return app.exec();
}
