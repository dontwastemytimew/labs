#include "datamanager.h"
#include "json.hpp"
#include <fstream>
#include <QList>
#include <QString>
#include <QDebug>

using json = nlohmann::json;

DataManager::DataManager() {
    Schema bookSchema("Книга");
    bookSchema.addField({"Автор", "Текст"});
    bookSchema.addField({"Рік видання", "Число"});
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

    std::ofstream file(filePath.toStdString());
    if (file.is_open()) {
        file << rootObject.dump(4);
    }
}

void DataManager::loadFromFile(const QString &filePath) {
    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) {
        return;
    }

    json rootObject;
    try {
        file >> rootObject;
    } catch (...) {
        return;
    }

    m_schemas.clear();
    m_notes.clear();

    // Завантажуємо схеми
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

    // Завантажуємо нотатки
        for (const auto& noteObject : rootObject["notes"]) {
            Note newNote(
                QString::fromStdString(noteObject["title"]),
                noteObject["schemaId"]
            );
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

void DataManager::removeNote(int index) {
    if (index >= 0 && index < m_notes.size()) {
        m_notes.removeAt(index);
    }
}

void DataManager::exportNote(int index, const QString& filePath) const {
    if (index < 0 || index >= m_notes.size()) return;

    json noteObject;
    const Note& note = m_notes[index];

    // Зберігаємо лише цю одну нотатку
    noteObject["title"] = note.getTitle().toStdString();
    noteObject["schemaId"] = note.getSchemaId();
    noteObject["tags"] = json::array();
    for (const auto& tag : note.getTags()) {
        noteObject["tags"].push_back(tag.toStdString());
    }
    noteObject["fields"] = json::object();
    for (auto it = note.getFields().constBegin(); it != note.getFields().constEnd(); ++it) {
        noteObject["fields"][it.key().toStdString()] = it.value().toStdString();
    }

    // Записуємо JSON-об'єкт у файл
    std::ofstream file(filePath.toStdString());
    if (file.is_open()) {
        file << noteObject.dump(4);
    }
}

void DataManager::importNote(const QString& filePath) {
    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) return;

    json noteObject;
    try {
        file >> noteObject;
    } catch (...) {
        return;
    }

    QString title = QString::fromStdString(noteObject.value("title", "Імпортована нотатка"));
    int schemaId = noteObject.value("schemaId", 0); // Беремо ID схеми

    Note importedNote(title, schemaId);

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

    m_notes.append(importedNote);
}