#pragma once
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QSet>

class Note {
public:
    Note(const QString& title, int schemaId);

    void addField(const QString& name, const QString& value);

    QString getTitle() const;
    int getSchemaId() const;
    const QMap<QString, QString>& getFields() const;
    QDateTime getCreationDate() const;

    void addTag(const QString& tag);
    const QSet<QString>& getTags() const;

    void setTags(const QSet<QString>& tags);

    void setTitle(const QString& title);

private:
    QString m_title;
    int m_schemaId; // ID схеми, за якою створено нотатку
    // Зберігаємо дані полів як "Назва поля" -> "Значення"
    QMap<QString, QString> m_fields;
    QDateTime m_creationDate;
    QSet<QString> m_tags;
};