#pragma once
#include <QString>
#include <QMap>

class Note {
public:
    Note(const QString& title, int schemaId);

    void addField(const QString& name, const QString& value);

    QString getTitle() const;
    int getSchemaId() const;
    const QMap<QString, QString>& getFields() const;

private:
    QString m_title;
    int m_schemaId; // ID схеми, за якою створено нотатку
    // Зберігаємо дані полів як "Назва поля" -> "Значення"
    QMap<QString, QString> m_fields;
};