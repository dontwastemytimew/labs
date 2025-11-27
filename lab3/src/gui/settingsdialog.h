#pragma once
#include <QDialog>
#include <QEvent>

namespace Ui { class SettingsDialog; }

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    private slots:
        void on_runBenchmarkButton_clicked();
    void on_themeComboBox_currentIndexChanged(int index);

    void on_languageComboBox_currentIndexChanged(int index);

    protected:
    void changeEvent(QEvent *event) override;


private:
    Ui::SettingsDialog *ui;

    void plotResults(double nlohmannTime, double rapidJsonTime);
    void setDarkTheme(bool enable);

};