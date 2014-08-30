#include <QtPlugin>
#include <QtWidgets>
#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

//Q_IMPORT_PLUGIN(qwindows)
//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qpng)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
