#include <QApplication>
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "logger.h"


int main(int argc, char *argv[]) {
    Logger::init();

    QApplication a(argc, argv);

    QTranslator translator;
    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + locale,
                          QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    {
        a.installTranslator(&qtTranslator);
    }
    QString translationFileName = "lab2_" + locale;
    if (translator.load(translationFileName, "../i18n")) {
        a.installTranslator(&translator);
        qDebug() << "Завантажено переклад програми для локалізації:" << locale;
    } else {
        qDebug() << "Не вдалося завантажити переклад програми для локалізації:" << locale;
    }

    qInfo() << QObject::tr("Програма запущена.");
    MainWindow w;

    w.updateNotesList();

    w.show();
    int result = a.exec();
    qInfo() << QObject::tr("Застосунок завершено з кодом виходу %1").arg(result);
    return result;
}