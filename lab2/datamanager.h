#pragma once
#include "schema.h"
#include <QList>
#include "note.h"
#include <QString>

/**
 * @class DataManager
 * @brief Центральний клас для керування всіма даними програми.
 *
 * Відповідає за зберігання, завантаження, додавання, оновлення та видалення
 * схем і нотаток. Виступає як єдине джерело даних ("мозок") для всього GUI.
 */
class DataManager {
public:
    /**
     * @brief Конструктор. Створює одну тестову схему "Книга" для демонстрації.
     */
    DataManager();

    // --- Методи для роботи зі схемами ---

    /**
     * @brief Додає нову схему до сховища.
     * @param schema Об'єкт схеми для додавання.
     */
    void addSchema(const Schema& schema);

 /**
     * @brief Повертає список усіх схем.
     * @return Константне посилання на QList об'єктів Schema.
     */
    const QList<Schema>& getSchemas() const;

 /**
     * @brief Видаляє схему зі сховища за її індексом.
     * @param index Індекс схеми, яку потрібно видалити.
     */
    void removeSchema(int index);

    /**
     * @brief Оновлює існуючу схему за її індексом.
     * @param index Індекс схеми у списку, яку потрібно оновити.
     * @param schema Новий об'єкт схеми, який замінить старий.
     */
    void updateSchema(int index, const Schema& schema);

 // --- Методи для роботи з нотатками ---

 /**
  * @brief Додає нову нотатку до сховища.
  * @param note Об'єкт нотатки для додавання.
  */
    void addNote(const Note& note);

 /**
     * @brief Повертає список усіх нотаток для читання та зміни.
     * @return Посилання на QList об'єктів Note.
     */
    QList<Note>& getNotes();

 /**
     * @brief Видаляє нотатку зі сховища за її індексом.
     * @param index Індекс нотатки, яку потрібно видалити.
     */
    void removeNote(int index);

 /**
     * @brief Оновлює існуючу нотатку за її індексом.
     * @param index Індекс нотатки, яку потрібно оновити.
     * @param note Новий об'єкт нотатки для заміни.
     */
    void updateNote(int index, const Note& note);

 // --- Методи для збереження, завантаження, експорту та імпорту ---

 /**
  * @brief Зберігає всі схеми та нотатки у файл JSON.
  * @param filePath Шлях до файлу для збереження.
  */
    void saveToFile(const QString& filePath) const;

 /**
     * @brief Завантажує всі схеми та нотатки з файлу JSON.
     * @param filePath Шлях до файлу для завантаження.
     */
    void loadFromFile(const QString& filePath);

 /**
     * @brief Експортує одну конкретну нотатку у файл JSON.
     * @param index Індекс нотатки, яку потрібно експортувати.
     * @param filePath Шлях до файлу для експорту.
     */
    void exportNote(int index, const QString& filePath) const;

 /**
     * @brief Імпортує нотатку з файлу JSON і додає її до сховища.
     * @param filePath Шлях до файлу для імпорту.
     */
    void importNote(const QString& filePath);

private:
 QList<Schema> m_schemas; ///< Внутрішнє сховище для списку всіх схем.
 QList<Note> m_notes;     ///< Внутрішнє сховище для списку всіх нотаток.
};