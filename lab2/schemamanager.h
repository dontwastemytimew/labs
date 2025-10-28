#pragma once
#include <QWidget>
#include "datamanager.h"
#include <QDialog>

namespace Ui { class SchemaManager; }

class SchemaManager : public QDialog {
    Q_OBJECT

public:
    explicit SchemaManager(DataManager *dataManager, QWidget *parent = nullptr);
    ~SchemaManager();

    private slots:
    void openSchemaEditor();
    void on_deleteSchemaButton_clicked();
    void on_editSchemaButton_clicked();
    void on_closeButton_clicked();

private:
    void updateSchemasList();
    Ui::SchemaManager *ui;
    DataManager *m_dataManager;
};
