#include "schemamanager.h"
#include "ui_schemamanager.h"
#include "schemaeditor.h"
#include <QDialog>
#include <QPushButton>
#include <QWidget>

SchemaManager::SchemaManager(DataManager *dataManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SchemaManager),
    m_dataManager(dataManager)
{
    ui->setupUi(this);
    setWindowTitle("Керування схемами");

    connect(ui->createSchemaButton, &QPushButton::clicked, this, &SchemaManager::openSchemaEditor);

    updateSchemasList();
}

SchemaManager::~SchemaManager() {
    delete ui;
}

void SchemaManager::updateSchemasList()
{
    ui->schemasListWidget->clear();

    for (const auto& schema : m_dataManager->getSchemas()) {
        ui->schemasListWidget->addItem(schema.getName());
    }
}

void SchemaManager::openSchemaEditor()
{
    SchemaEditor editor(this);

    if (editor.exec() == QDialog::Accepted)
    {
        Schema newSchema = editor.getSchema();

        m_dataManager->addSchema(newSchema);

        updateSchemasList();
    }
}

void SchemaManager::on_deleteSchemaButton_clicked()
{
    int currentIndex = ui->schemasListWidget->currentRow();

    if (currentIndex < 0) {
        return;
    }

    m_dataManager->removeSchema(currentIndex);

    updateSchemasList();
}

void SchemaManager::on_editSchemaButton_clicked()
{
    int currentIndex = ui->schemasListWidget->currentRow();
    if (currentIndex < 0) {
        return;
    }

    const Schema& schemaToEdit = m_dataManager->getSchemas()[currentIndex];

    SchemaEditor editor(schemaToEdit, this);

    if (editor.exec() == QDialog::Accepted)
    {

        Schema updatedSchema = editor.getSchema();
        m_dataManager->updateSchema(currentIndex, updatedSchema);
        updateSchemasList();
    }
}

void SchemaManager::on_closeButton_clicked()
{
    this->close();
}