#pragma once
#include <QMainWindow>
#include "datamanager.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
    void on_manageSchemasButton_clicked();
    void on_createNoteButton_clicked();
    void updateNotesList();


private:
    Ui::MainWindow *ui;
    DataManager m_dataManager;
};