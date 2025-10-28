#include "schemaeditor.h"
#include "ui_schemaeditor.h"
#include "schemaeditor.h"
#include <QMessageBox>

SchemaEditor::SchemaEditor(QWidget *parent) :
    QDialog(parent), ui(new Ui::SchemaEditor) {
    ui->setupUi(this);
    setWindowTitle("Редактор схеми");
}

SchemaEditor::~SchemaEditor() {
    delete ui;
}


Schema SchemaEditor::getSchema() const
{
    Schema newSchema(ui->schemaNameLineEdit->text());

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

SchemaEditor::SchemaEditor(const Schema &schemaToEdit, QWidget *parent) :
    QDialog(parent), ui(new Ui::SchemaEditor) {
    ui->setupUi(this);
    setWindowTitle("Редактор схеми");

    ui->schemaNameLineEdit->setText(schemaToEdit.getName());

    for (const auto& field : schemaToEdit.getFields()) {
        m_tempFields.append(field);
        ui->fieldsListWidget->addItem(field.name);
    }
}

void SchemaEditor::accept()
{

    if (ui->schemaNameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Помилка валідації", "Назва схеми не може бути порожньою!");
    } else {
        QDialog::accept();
    }
}

