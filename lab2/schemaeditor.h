#pragma once
#include <QDialog>
#include "schema.h"

namespace Ui { class SchemaEditor; }

class SchemaEditor : public QDialog {
    Q_OBJECT

public:
    explicit SchemaEditor(QWidget *parent = nullptr);

    explicit SchemaEditor(const Schema& schemaToEdit, QWidget *parent = nullptr);

    ~SchemaEditor();

    Schema getSchema() const;

    private slots:
    void on_addFieldButton_clicked();
    void accept() override;

private:
    Ui::SchemaEditor *ui;
    QList<Field> m_tempFields;
};