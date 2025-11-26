#pragma once
#include <QDialog>
#include "schema.h"

namespace Ui { class SchemaEditor; }

/**
 * @class SchemaEditor
 * @brief Діалогове вікно для створення та редагування однієї схеми.
 *
 * Надає користувачеві поля для введення назви схеми та для додавання
 * полів, які будуть визначати структуру нотаток.
 */
class SchemaEditor : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор для створення нової схеми.
     * @param parent Вказівник на батьківський віджет.
     */
    explicit SchemaEditor(QWidget *parent = nullptr);

    /**
     * @brief Конструктор для редагування існуючої схеми.
     * @param schemaToEdit Схема, дані якої потрібно завантажити в редактор.
     * @param parent Вказівник на батьківський віджет.
     */
    explicit SchemaEditor(const Schema& schemaToEdit, QWidget *parent = nullptr);

    ~SchemaEditor();

    /**
     * @brief Збирає дані з полів вводу та повертає готовий об'єкт Schema.
     * @return Створений або оновлений об'єкт Schema.
     */
    Schema getSchema() const;

    private slots:
 /**
     * @brief Слот, що викликається при натисканні на кнопку "Додати поле".
     *
     * Додає нове поле до тимчасового списку та оновлює GUI.
     */
    void on_addFieldButton_clicked();

    /**
     * @brief Перевизначений слот, що викликається при натисканні "OK".
     *
     * Виконує валідацію (перевірку), що назва схеми не порожня,
     * перед тим, як закрити вікно.
     */
    void accept() override;

 protected:
 void changeEvent(QEvent *event) override;

private:
 Ui::SchemaEditor *ui;        ///< Вказівник на об'єкти інтерфейсу.
 QList<Field> m_tempFields; ///< Тимчасове сховище для полів, які додає користувач.
};