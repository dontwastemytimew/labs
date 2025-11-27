#include "datamanager.h"
#include "json.hpp"
#include <QList>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QFile>

using json = nlohmann::json;

DataManager::DataManager() {
    Schema bookSchema(QObject::tr("Книга"));
    bookSchema.addField({QObject::tr("Автор"), QObject::tr("Текст")});
    bookSchema.addField({QObject::tr("Рік видання"), QObject::tr("Число")});
    m_schemas.append(bookSchema);
}

void DataManager::addSchema(const Schema &schema) {
    m_schemas.append(schema);
}

const QList<Schema>& DataManager::getSchemas() const {
    return m_schemas;
}

void DataManager::removeSchema(int index) {
    if (index >= 0 && index < m_schemas.size()) {
        m_schemas.removeAt(index);
    }
}

void DataManager::updateSchema(int index, const Schema& schema) {
    if (index >= 0 && index < m_schemas.size()) {
        m_schemas[index] = schema;
    }
}

void DataManager::addNote(const Note &note) {
    m_notes.append(note);
}

QList<Note>& DataManager::getNotes() {
    return m_notes;
}

void DataManager::updateNote(int index, const Note& note) {
    if (index >= 0 && index < m_notes.size()) {
        m_notes[index] = note;
    }
}

void DataManager::saveToFile(const QString &filePath) const {
    json rootObject;
    rootObject["schemas"] = json::array();

    for (const auto& schema : m_schemas) {
        json schemaObject;
        schemaObject["name"] = schema.getName().toStdString();
        schemaObject["fields"] = json::array();
        for (const auto& field : schema.getFields()) {
            json fieldObject;
            fieldObject["name"] = field.name.toStdString();
            fieldObject["type"] = field.type.toStdString();
            schemaObject["fields"].push_back(fieldObject);
        }
        rootObject["schemas"].push_back(schemaObject);
    }

    rootObject["notes"] = json::array();
    for (const auto& note : m_notes) {
        json noteObject;
        noteObject["title"] = note.getTitle().toStdString();
        noteObject["schemaId"] = note.getSchemaId();

        noteObject["image"] = note.getImage().toStdString();

        noteObject["fields"] = json::object();
        const auto& fields = note.getFields();
        for (auto it = fields.constBegin(); it != fields.constEnd(); ++it) {
            noteObject["fields"][it.key().toStdString()] = it.value().toStdString();
        }

        noteObject["tags"] = json::array();
        for (const auto& tag : note.getTags()) {
            noteObject["tags"].push_back(tag.toStdString());
        }
        rootObject["notes"].push_back(noteObject);
    }
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(rootObject.dump(4));
        file.close();
        qInfo() << QObject::tr("Дані успішно збережено у файл:") << filePath;
    } else {
        qCritical() << QObject::tr("ПОМИЛКА: не вдалося відкрити файл для збереження:") << filePath;
    }
}

void DataManager::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << QObject::tr("Файл даних не знайдено. Буде створено новий при закритті.");
        return;
    }

    QString content = file.readAll();
    file.close();

    json rootObject;
    try {
        rootObject = json::parse(content.toStdString());
    } catch (const std::exception& e) {
        qCritical() << QObject::tr("ПОМИЛКА: Не вдалося розпарсити JSON файл:") << filePath << QObject::tr("Помилка:") << e.what();
        return;
    }

    m_schemas.clear();
    m_notes.clear();

    if (rootObject.contains("schemas")) {
        for (const auto& schemaObject : rootObject["schemas"]) {
            Schema newSchema(QString::fromStdString(schemaObject["name"]));
            if (schemaObject.contains("fields")) {
                for (const auto& fieldObject : schemaObject["fields"]) {
                    newSchema.addField({
                        QString::fromStdString(fieldObject["name"]),
                        QString::fromStdString(fieldObject["type"])
                    });
                }
            }
            m_schemas.append(newSchema);
        }
    }

    if (rootObject.contains("notes")) {
        for (const auto& noteObject : rootObject["notes"]) {
            Note newNote(
                QString::fromStdString(noteObject["title"]),
                noteObject["schemaId"]
            );

            if (noteObject.contains("image")) {
                newNote.setImage(QString::fromStdString(noteObject["image"]));
            }

            if (noteObject.contains("fields")) {
                for (auto& [key, value] : noteObject["fields"].items()) {
                    newNote.addField(QString::fromStdString(key), QString::fromStdString(value));
                }
            }
            if (noteObject.contains("tags")) {
                for (const auto& tag : noteObject["tags"]) {
                    newNote.addTag(QString::fromStdString(tag));
                }
            }
            m_notes.append(newNote);
        }
    }

    qInfo() << QObject::tr("Дані успішно завантажено. Знайдено %1 схем та %2 нотаток.").arg(m_schemas.size()).arg(m_notes.size());
}

void DataManager::removeNote(int index) {
    if (index >= 0 && index < m_notes.size()) {
        m_notes.removeAt(index);
    }
}

void DataManager::exportNote(int index, const QString& filePath) const {
    if (index < 0 || index >= m_notes.size()) return;

    json noteObject;
    const Note& note = m_notes[index];

    noteObject["title"] = note.getTitle().toStdString();
    noteObject["schemaId"] = note.getSchemaId();

    noteObject["image"] = note.getImage().toStdString();

    noteObject["tags"] = json::array();
    for (const auto& tag : note.getTags()) {
        noteObject["tags"].push_back(tag.toStdString());
    }
    noteObject["fields"] = json::object();
    for (auto it = note.getFields().constBegin(); it != note.getFields().constEnd(); ++it) {
        noteObject["fields"][it.key().toStdString()] = it.value().toStdString();
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(noteObject.dump(4));
        file.close();
        qInfo() << QObject::tr("Нотатку '%1' успішно експортовано у файл: %2").arg(note.getTitle()).arg(filePath);
    } else {
        qCritical() << QObject::tr("ПОМИЛКА: Не вдалося відкрити файл для експорту:") << filePath;
    }
}


void DataManager::importNote(const QString& filePath) {
    qDebug() << "--- Початок імпорту з файлу:" << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << QObject::tr("ПОМИЛКА: Не вдалося відкрити файл для імпорту: %1").arg(filePath);
        return;
    }

    QString content = file.readAll();
    file.close();

    json noteObject;
    try {
        noteObject = json::parse(content.toStdString());
        qDebug() << "JSON файл успішно розпарсено.";
    } catch (const std::exception& e) {
        qCritical() << QObject::tr("ПОМИЛКА: JSON файл пошкоджено! %1").arg(e.what());
        return;
    }

    QString title = QString::fromStdString(noteObject.value("title", QObject::tr("Імпортована нотатка").toStdString()));
    int schemaId = noteObject.value("schemaId", 0);

    Note importedNote(title, schemaId);

    if (noteObject.contains("image")) {
        importedNote.setImage(QString::fromStdString(noteObject["image"]));
    }

    if (noteObject.contains("fields")) {
        for (auto& [key, value] : noteObject["fields"].items()) {
            importedNote.addField(QString::fromStdString(key), QString::fromStdString(value));
        }
    }

    if (noteObject.contains("tags")) {
        QSet<QString> tags;
        for (const auto& tag : noteObject["tags"]) {
            tags.insert(QString::fromStdString(tag));
        }
        importedNote.setTags(tags);
    }

    qDebug() << QObject::tr("Нотатка для додавання готова. Назва: %1").arg(importedNote.getTitle());

    m_notes.append(importedNote);
    qInfo() << QObject::tr("Нотатку успішно імпортовано з файлу: %1").arg(filePath);
    qDebug() << QObject::tr("Кількість нотаток у DataManager тепер: %1").arg(m_notes.size());
}