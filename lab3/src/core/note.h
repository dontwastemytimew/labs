#pragma once
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QSet>

/**
 * @class Note
 * @brief Модель даних для однієї нотатки.
 *
 * Зберігає всю інформацію, що стосується однієї нотатки: її назву,
 * ID схеми, за якою її створено, поля з даними, теги та дату створення.
 */
class Note {
public:
    /**
     * @brief Конструктор класу Note.
     * @param title Назва нової нотатки.
     * @param schemaId Ідентифікатор (індекс) схеми, за якою створюється нотатка.
     */
    Note(const QString& title, int schemaId);

    /**
       * @brief Додає нове поле з даними до нотатки.
       * @param name Назва поля (має відповідати назві поля у схемі).
       * @param value Значення, яке ввів користувач.
       */
    void addField(const QString& name, const QString& value);

    /**
     * @brief Повертає назву нотатки.
     * @return QString з назвою.
     */
    QString getTitle() const;

 /**
     * @brief Повертає ID схеми, до якої належить нотатка.
     * @return int Ідентифікатор (індекс) схеми.
     */
    int getSchemaId() const;

 /**
     * @brief Повертає мапу полів та їх значень.
     * @return Константне посилання на QMap<QString, QString>.
     */
    const QMap<QString, QString>& getFields() const;

 /**
     * @brief Повертає дату та час створення нотатки.
     * @return QDateTime об'єкт з датою створення.
     */
    QDateTime getCreationDate() const;

 /**
     * @brief Додає один тег до нотатки.
     * @param tag Текст тега для додавання.
     */
    void addTag(const QString& tag);

 /**
     * @brief Повертає набір тегів, що належать до нотатки.
     * @return Константне посилання на QSet<QString>.
     */
    const QSet<QString>& getTags() const;

    /**
         * @brief Повністю замінює набір тегів нотатки.
         * @param tags Новий набір тегів.
         */
    void setTags(const QSet<QString>& tags);

    /**
     * @brief Встановлює нову назву для нотатки.
     * @param title Нова назва.
     */
    void setTitle(const QString& title);

private:
    QString m_title;                ///< Назва нотатки.
    int m_schemaId;                 ///< ID схеми, за якою створено нотатку.
    QMap<QString, QString> m_fields;///< Дані полів у форматі "Назва поля" -> "Значення".
    QDateTime m_creationDate;       ///< Дата та час створення нотатки.
    QSet<QString> m_tags;           ///< Набір унікальних тегів.
};