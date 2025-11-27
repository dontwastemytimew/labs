#pragma once

#include <QString>
#include <QList>

/**
 * @struct Field
 * @brief Описує одне поле в схемі (наприклад, "Назва", "Автор").
 */
struct Field {
    QString name; ///< Назва поля, яку бачить користувач.
    QString type; ///< Тип поля (наприклад, "Текст", "Число").
};

/**
 * @class Schema
 * @brief Модель даних для однієї схеми.
 *
 * Клас зберігає назву схеми та список полів, які вона визначає.
 * Це "креслення" для майбутніх нотаток.
 */
class Schema {
public:
    /**
     * @brief Конструктор класу Schema.
     * @param name Назва нової схеми.
     */
    Schema(const QString &name);

    /**
     * @brief Додає нове поле до схеми.
     * @param field Об'єкт Field, що містить назву та тип поля.
     */
    void addField(const Field &field);

    /**
     * @brief Повертає назву схеми.
     * @return QString з назвою схеми.
     */
    QString getName() const;

 /**
     * @brief Повертає список полів, що належать до цієї схеми.
     * @return Константне посилання на QList об'єктів Field.
     */
    const QList<Field>& getFields() const;

private:
    QString m_name;        ///< Внутрішнє сховище для назви схеми.
    QList<Field> m_fields; ///< Внутрішнє сховище для списку полів.
};