#include "datamanager.h"

DataManager::DataManager() {
    // Для тесту можна додати одну схему за замовчуванням
    m_schemas.append(Schema("Книга"));
}

void DataManager::addSchema(const Schema &schema) {
    m_schemas.append(schema);
}

const QList<Schema>& DataManager::getSchemas() const {
    return m_schemas;
}

void DataManager::addNote(const Note &note) {
    m_notes.append(note);
}

const QList<Note>& DataManager::getNotes() const {
    return m_notes;
}