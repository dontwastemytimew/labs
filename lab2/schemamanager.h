#pragma once
#include <QWidget>
#include "datamanager.h"

namespace Ui { class SchemaManager; }

class SchemaManager : public QWidget {
    Q_OBJECT

public:
    explicit SchemaManager(DataManager *dataManager, QWidget *parent = nullptr);
    ~SchemaManager();

    private slots:
    void on_createSchemaButton_clicked();

private:
    void updateSchemasList();
    Ui::SchemaManager *ui;
    DataManager *m_dataManager;
};
