#include "note.h"

Note::Note(const QString &title, int schemaId)
: m_title(title), m_schemaId(schemaId), m_creationDate(QDateTime::currentDateTime()) {
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

QDateTime Note::getCreationDate() const {
    return m_creationDate;
}

void Note::addTag(const QString& tag) {
    m_tags.insert(tag.trimmed());
}

const QSet<QString>& Note::getTags() const {
    return m_tags;
}

void Note::setTags(const QSet<QString>& tags) { m_tags = tags; }

void Note::setTitle(const QString &title) {
    m_title = title;
}