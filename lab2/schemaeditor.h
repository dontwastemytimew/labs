#pragma once
#include <QDialog>
#include "schema.h"

namespace Ui { class SchemaEditor; }

class SchemaEditor : public QDialog {
    Q_OBJECT

public:
    explicit SchemaEditor(QWidget *parent = nullptr);
    ~SchemaEditor();

    Schema getSchema() const;

    private slots:
    void on_createSchemaButton_clicked();
    void on_addFieldButton_clicked();

private:
    Ui::SchemaEditor *ui;
    QList<Field> m_tempFields;
};