#include "note.h"

Note::Note(const QString &title, int schemaId)
    : m_title(title), m_schemaId(schemaId) {
}

QString Note::getTitle() const {
    return m_title;
}

int Note::getSchemaId() const {
    return m_schemaId;
}

const QMap<QString, QString>& Note::getFields() const {
    return m_fields;
}

void Note::addField(const QString& name, const QString& value) {
    m_fields.insert(name, value);
}