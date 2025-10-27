#include "schemamanager.h"
#include "ui_schemamanager.h"
#include "schemaeditor.h"

SchemaManager::SchemaManager(DataManager *dataManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchemaManager),
    m_dataManager(dataManager)
{
    ui->setupUi(this);
    setWindowTitle("Керування схемами");
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

void SchemaManager::on_createSchemaButton_clicked()
{
    SchemaEditor editor(this);

    if (editor.exec() == QDialog::Accepted)
    {
        Schema newSchema = editor.getSchema();

        m_dataManager->addSchema(newSchema);

        updateSchemasList();
    }
}