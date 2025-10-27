#include "schemaeditor.h"
#include "ui_schemaeditor.h"
#include "schemaeditor.h"

SchemaEditor::SchemaEditor(QWidget *parent) :
    QDialog(parent), ui(new Ui::SchemaEditor) {
    ui->setupUi(this);
    setWindowTitle("Редактор схеми");
}

SchemaEditor::~SchemaEditor() {
    delete ui;
}

void SchemaEditor::on_createSchemaButton_clicked()
{
    qDebug() << "Create button was clicked inside SchemaEditor!";
    // Тут буде логіка створення схеми
}

Schema SchemaEditor::getSchema() const
{
    // Створюємо схему з назвою
    Schema newSchema(ui->schemaNameLineEdit->text());

    // Додаємо до неї всі поля, які користувач ввів
    for (const auto& field : m_tempFields) {
        newSchema.addField(field);
    }

    return newSchema;
}

void SchemaEditor::on_addFieldButton_clicked() {
    QString fieldName = ui->fieldNameLineEdit->text();

    if (!fieldName.isEmpty()) {
        Field newField;
        newField.name = fieldName;
        newField.type = "Текст"; // Поки що тип поля завжди "Текст" для простоти

        m_tempFields.append(newField);

        ui->fieldsListWidget->addItem(fieldName);

        ui->fieldNameLineEdit->clear();
    }
}

