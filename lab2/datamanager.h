#pragma once
#include "schema.h"
#include <QList>
#include "note.h"

class DataManager {
public:
    DataManager();


    // Методи для роботи зі схемами
    void addSchema(const Schema& schema);
    const QList<Schema>& getSchemas() const;

    void addNote(const Note& note);
    const QList<Note>& getNotes() const;

private:
    QList<Schema> m_schemas; // Список, де зберігатимуться всі схеми
    QList<Note> m_notes;
};