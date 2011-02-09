#include <QtGui>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    /* ��������� ������ � ���������� */
    Q_INIT_RESOURCE(images);

    /* ������������� ��������� ���������� */
    QTextCodec *cp1251 = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForTr(cp1251);

    /* ������� ���������� */
    QApplication app(argc, argv);

    /* ��������� ����������� ����� ���� ������ */
    if (!QSystemTrayIcon::isSystemTrayAvailable()) 
        {
        QMessageBox::critical(
			0, 
			QObject::tr("Systray"), 
			QObject::tr("I couldn't detect any system tray on this system.")
			);
        return 1;
        }

    /* ��������� ������� �����, ������ �� ���� ������ � ������ */
    QApplication::setQuitOnLastWindowClosed(false);

    /* ������� ���� � ���������� ���, ��������� ���������� */
    MainWindow w;
    w.show();
    return app.exec();
}
