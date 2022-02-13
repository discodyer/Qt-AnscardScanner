#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":/../ReaderUI_zh_CN.qm");
    a.installTranslator(&translator);
//    const QStringList uiLanguages = QLocale::system().uiLanguages();
//    for (const QString &locale : uiLanguages) {
//        const QString baseName = "ReaderUI_" + QLocale(locale).name();
//        if (translator.load(":/i18n/" + baseName)) {
//            a.installTranslator(&translator);
//            break;
//        }
//    }
//    QFont font = a.font();
//    font.setPixelSize(12);
//    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
