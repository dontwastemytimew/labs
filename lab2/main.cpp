#include <QApplication>
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QFile file("log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    QString logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
        case QtDebugMsg:    logMessage += QObject::tr("[DEBUG] ");   break;
        case QtInfoMsg:     logMessage += QObject::tr("[INFO] ");    break;
        case QtWarningMsg:  logMessage += QObject::tr("[WARNING] "); break;
        case QtCriticalMsg: logMessage += QObject::tr("[CRITICAL] ");break;
        case QtFatalMsg:    logMessage += QObject::tr("[FATAL] ");   break;
    }

    logMessage += msg;
    out << logMessage << "\n";
    file.close();
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(messageHandler);

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