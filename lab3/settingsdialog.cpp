#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "nanobench.h"
#include "qcustomplot.h"
#include <QElapsedTimer>
#include <vector>
#include <string>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

static QTranslator *appTranslator = nullptr;
static bool isDarkThemeActive = false;

using nlohmann_json = nlohmann::json;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Налаштування"));
    ui->tabWidget->setCurrentIndex(0);

    ui->languageComboBox->addItem(tr("Українська"), QVariant("uk_UA"));
    ui->languageComboBox->addItem(tr("English"), QVariant("en_US"));

    ui->languageComboBox->blockSignals(true);
    int langIdx = appTranslator ? 1 : 0;
    ui->languageComboBox->setCurrentIndex(langIdx);
    ui->languageComboBox->blockSignals(false);

    ui->themeComboBox->blockSignals(true);

    ui->themeComboBox->addItem(tr("Світла"));
    ui->themeComboBox->addItem(tr("Темна"));
    ui->themeComboBox->setCurrentIndex(isDarkThemeActive ? 1 : 0);
    ui->themeComboBox->blockSignals(false);

    ui->plotWidget->xAxis->setLabel(tr("Бібліотека"));
    ui->plotWidget->yAxis->setLabel(tr("Час (мс)"));
    ui->plotWidget->legend->setVisible(true);

    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::on_themeComboBox_currentIndexChanged);
    connect(ui->languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::on_languageComboBox_currentIndexChanged);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::on_runBenchmarkButton_clicked()
{
    ui->runBenchmarkButton->setEnabled(false);
    ui->runBenchmarkButton->setText(tr("Тестування..."));
    ui->resultLabel->setText(tr("Генерація даних..."));
    qApp->processEvents();

    int count = 100000;
    std::vector<std::string> testData;
    testData.reserve(count);
    for(int i=0; i<count; ++i) {
        testData.push_back("Test Note Content " + std::to_string(i));
    }

    ui->resultLabel->setText(tr("Тест nlohmann..."));
    qApp->processEvents();

    // Тест nlohmann
    QElapsedTimer timer;
    timer.start();
    {
        nlohmann_json root;
        root["notes"] = nlohmann_json::array();
        for(const auto& str : testData) {
            nlohmann_json obj;
            obj["title"] = str;
            root["notes"].push_back(obj);
        }
        std::string res = root.dump();
    }
    double nlohmannTime = timer.elapsed();

    ui->resultLabel->setText(tr("Тест RapidJSON..."));
    qApp->processEvents();

    // Тест RapidJSON
    timer.restart();
    {
        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("notes");
        writer.StartArray();
        for(const auto& str : testData) {
            writer.StartObject();
            writer.Key("title");
            writer.String(str.c_str());
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
        std::string res = s.GetString();
    }
    double rapidJsonTime = timer.elapsed();

    plotResults(nlohmannTime, rapidJsonTime);

    ui->runBenchmarkButton->setEnabled(true);
    ui->runBenchmarkButton->setText(tr("Запустити тест"));

    QString winner = (rapidJsonTime < nlohmannTime) ? "RapidJSON" : "nlohmann";
    double diff = (nlohmannTime > rapidJsonTime) ? nlohmannTime/rapidJsonTime : rapidJsonTime/nlohmannTime;

    ui->resultLabel->setText(tr("Переміг: %1 (швидше в %2 разів!)")
                             .arg(winner).arg(diff, 0, 'f', 2));
}

void SettingsDialog::plotResults(double nlohmannTime, double rapidJsonTime)
{
    ui->plotWidget->clearPlottables();
    ui->plotWidget->clearItems();

    QCPBars *bars = new QCPBars(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
    bars->setName(tr("Швидкість серіалізації"));
    bars->setWidth(0.6);

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2;
    labels << "nlohmann/json" << "RapidJSON";

    QVector<double> keys, values;
    keys << 1 << 2;
    values << nlohmannTime << rapidJsonTime;
    bars->setData(keys, values);


    QColor loserColor("#d8bfd8");

    QColor winnerColor("#ff1493");

    QCPBars *winnerBar = new QCPBars(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
    winnerBar->setWidth(0.6);
    winnerBar->setPen(Qt::NoPen);
    winnerBar->setBrush(winnerColor);

    if (rapidJsonTime < nlohmannTime) {
        winnerBar->addData(2, rapidJsonTime);
        bars->setBrush(loserColor);
    } else {
        winnerBar->addData(1, nlohmannTime);
        bars->setBrush(loserColor);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->plotWidget->xAxis->setTicker(textTicker);
    ui->plotWidget->xAxis->setRange(0, 3);

    double maxVal = std::max(nlohmannTime, rapidJsonTime);
    ui->plotWidget->yAxis->setRange(0, maxVal * 1.3);
    ui->plotWidget->yAxis->setLabel(tr("Час виконання (мілісекунди)"));

    auto addLabel = [&](double x, double y, const QColor& color) {
        QCPItemText *text = new QCPItemText(ui->plotWidget);
        text->position->setCoords(x, y);
        text->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        text->setText(QString::number(y, 'f', 1) + " ms");
        text->setFont(QFont("Arial", 10, QFont::Bold));
        text->setColor(color);
    };

    addLabel(1, nlohmannTime, QColor("#804060"));

    addLabel(2, rapidJsonTime, winnerColor);

    ui->plotWidget->replot();
}

void SettingsDialog::on_languageComboBox_currentIndexChanged(int index)
{
    QString langCode = ui->languageComboBox->itemData(index).toString();
    if (langCode.isEmpty()) return;

    if (appTranslator) {
        qApp->removeTranslator(appTranslator);
        delete appTranslator;
        appTranslator = nullptr;
    }

    if (langCode == "en_US") {
        appTranslator = new QTranslator(qApp);
        if (appTranslator->load(":/i18n/lab3_en_US.qm")) {
            qApp->installTranslator(appTranslator);
            qInfo() << "Language changed to English";
        } else {
            qWarning() << "Failed to load English translation";
        }
    } else {
        qInfo() << "Language changed to Ukrainian (Default)";
        QEvent languageChangeEvents(QEvent::LanguageChange);
        QCoreApplication::sendEvent(qApp, &languageChangeEvents);
    }
}

void SettingsDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->themeComboBox->blockSignals(true);
        ui->languageComboBox->blockSignals(true);

        ui->retranslateUi(this);
        setWindowTitle(tr("Налаштування"));

        ui->themeComboBox->clear();
        ui->themeComboBox->addItem(tr("Світла"));
        ui->themeComboBox->addItem(tr("Темна"));

        ui->themeComboBox->setCurrentIndex(isDarkThemeActive ? 1 : 0);

        QVariant currentLangData = ui->languageComboBox->currentData();
        ui->languageComboBox->clear();
        ui->languageComboBox->addItem(tr("Українська"), "uk_UA");
        ui->languageComboBox->addItem(tr("English"), "en_US");
        int langIndex = ui->languageComboBox->findData(currentLangData);
        if (langIndex != -1) ui->languageComboBox->setCurrentIndex(langIndex);

        ui->plotWidget->xAxis->setLabel(tr("Бібліотека"));
        ui->plotWidget->yAxis->setLabel(tr("Час (мс)"));
        ui->plotWidget->replot();

        ui->themeComboBox->blockSignals(false);
        ui->languageComboBox->blockSignals(false);
    }
    QDialog::changeEvent(event);
}

void SettingsDialog::setDarkTheme(bool enable)
{
    isDarkThemeActive = enable;

    QString stylePath = enable ? ":/styles/styles/dark.qss" : ":/styles/styles/light.qss";
    QFile file(stylePath);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(QLatin1String(file.readAll()));
        file.close();
        qInfo() << "Тему змінено на:" << (enable ? "Dark" : "Light");
    } else {
        stylePath = enable ? ":/styles/dark.qss" : ":/styles/light.qss";
        QFile fileAlt(stylePath);
        if (fileAlt.open(QFile::ReadOnly | QFile::Text)) {
            qApp->setStyleSheet(QLatin1String(fileAlt.readAll()));
            fileAlt.close();
        }
    }
}


void SettingsDialog::on_themeComboBox_currentIndexChanged(int index)
{
    bool isDark = (index == 1);

    if (isDark == isDarkThemeActive) return;

    setDarkTheme(isDark);
}