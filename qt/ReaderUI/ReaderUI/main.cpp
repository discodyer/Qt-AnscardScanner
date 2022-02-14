#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
//    if (translator.load(QLocale::system(), u"ReaderUI"_qs, u"_"_qs, u":/i18n"_qs)){
//        app.installTranslator(&translator);
//    }
//    app.installTranslator(&translator);
    translator.load(":/ReaderUI_zh_CN.qm");
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
}
