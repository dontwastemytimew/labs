#pragma once

#include <QString>
#include <QList>

// Структура, що описує одне поле в схемі (наприклад, "Назва", "Автор")
struct Field {
    QString name;
    QString type; // Наприклад, "Текст", "Число", "Дата"
};

class Schema {
public:
    Schema(const QString &name);

    void addField(const Field &field);
    QString getName() const;
    const QList<Field>& getFields() const;

private:
    QString m_name;
    QList<Field> m_fields;
};