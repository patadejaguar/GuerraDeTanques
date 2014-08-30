#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFontDatabase>

//Q_IMPORT_PLUGIN(qwindows)
//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qpng)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/REVUEN.TTF");
    QFontDatabase::addApplicationFont(":/fonts/CONSTAN.TTF");
    QFontDatabase::addApplicationFont(":/fonts/CONSTANI.TTF");
    QFontDatabase::addApplicationFont(":/fonts/CONSTANB.TTF");
    QFontDatabase::addApplicationFont(":/fonts/CONSTANZ.TTF");

#ifdef Q_OS_WIN32
    QTextCodec *codec = QTextCodec::codecForName("ISO 8859-15");
    QTextCodec::setCodecForLocale(codec);
#endif
#ifdef Q_OS_UNIX
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    MainWindow w;
    w.showFullScreen();

    return a.exec();
}



