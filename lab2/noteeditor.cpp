#include "noteeditor.h"
#include "ui_noteeditor.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

NoteEditor::NoteEditor(DataManager *dataManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteEditor),
    m_dataManager(dataManager)
{
    ui->setupUi(this);
    setWindowTitle("Редактор нотатки");

    for (const auto& schema : m_dataManager->getSchemas()) {
        ui->schemaComboBox->addItem(schema.getName());
    }

    connect(ui->schemaComboBox, &QComboBox::currentIndexChanged, this, &NoteEditor::onSchemaSelected);
    onSchemaSelected(0);
}

NoteEditor::~NoteEditor() {
    delete ui;
}

void NoteEditor::onSchemaSelected(int index)
{
    // Отримуємо обрану схему з DataManager
    const auto& schemas = m_dataManager->getSchemas();
    if (index < 0 || index >= schemas.size()) return;
    const Schema& selectedSchema = schemas[index];

    if (ui->fieldsScrollArea->widget()) {
        delete ui->fieldsScrollArea->widget();
    }

    m_fieldInputs.clear();

    // Створюємо контейнер для наших майбутніх полів
    QWidget *fieldsContainer = new QWidget;
    QFormLayout *formLayout = new QFormLayout(fieldsContainer);

    // Створюємо QLabel та QLineEdit для кожного поля в схемі
    for (const auto& field : selectedSchema.getFields()) {
        QLabel *label = new QLabel(field.name);
        QLineEdit *lineEdit = new QLineEdit();
        formLayout->addRow(label, lineEdit);
        m_fieldInputs.append(lineEdit);
    }

    // Встановлюємо наш контейнер з полями всередину QScrollArea
    ui->fieldsScrollArea->setWidget(fieldsContainer);
}

Note NoteEditor::getNote() const
{
    // Отримуємо назву та обрану схему
    QString title = ui->titleLineEdit->text();
    int schemaIndex = ui->schemaComboBox->currentIndex();
    const Schema& selectedSchema = m_dataManager->getSchemas()[schemaIndex];

    // Створюємо нову нотатку
    Note newNote(title, schemaIndex);

    // Заповнюємо поля нотатки даними з QLineEdit
    for (int i = 0; i < selectedSchema.getFields().size(); ++i) {
        QString fieldName = selectedSchema.getFields()[i].name;
        QString fieldValue = m_fieldInputs[i]->text();
        newNote.addField(fieldName, fieldValue);
    }

    return newNote;
}