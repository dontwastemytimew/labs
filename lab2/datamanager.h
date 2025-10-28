#pragma once
#include "schema.h"
#include <QList>
#include "note.h"
#include <QString>

class DataManager {
public:
    DataManager();


    void addSchema(const Schema& schema);
    const QList<Schema>& getSchemas() const;
    void removeSchema(int index);
    void updateSchema(int index, const Schema& schema);

    void addNote(const Note& note);
    QList<Note>& getNotes();
    void removeNote(int index);
    void updateNote(int index, const Note& note);

    void saveToFile(const QString& filePath) const;
    void loadFromFile(const QString& filePath);

    void exportNote(int index, const QString& filePath) const;
    void importNote(const QString& filePath);

private:
    QList<Schema> m_schemas; // Список, де зберігатимуться всі схеми
    QList<Note> m_notes;
};