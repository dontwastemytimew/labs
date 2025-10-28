#pragma once
#include <QMainWindow>
#include "datamanager.h"
#include <QCloseEvent>
#include <QPoint>
#include <QListWidgetItem>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateNotesList();

    private slots:
    void on_manageSchemasButton_clicked();
    void on_createNoteButton_clicked();
    void deleteNoteLogic();
    void onNotesListContextMenuRequested(const QPoint &pos);
    void onNoteDoubleClicked(QListWidgetItem *item);
    void on_searchButton_clicked();
    void on_importNoteButton_clicked();


protected:
    void closeEvent(QCloseEvent *event) override;


private:
    Ui::MainWindow *ui;
    DataManager m_dataManager;
};