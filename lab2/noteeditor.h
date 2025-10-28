#pragma once
#include <QDialog>
#include "datamanager.h"
#include <QLineEdit>
#include "note.h"


namespace Ui { class NoteEditor; }

class NoteEditor : public QDialog {
    Q_OBJECT

public:
    explicit NoteEditor(DataManager *dataManager, QWidget *parent = nullptr);

    explicit NoteEditor(DataManager *dataManager, const Note& noteToEdit, QWidget *parent = nullptr);

    ~NoteEditor();
    Note getNote() const;

    private slots:
    void onSchemaSelected(int index);
    void accept() override;

private:
    Ui::NoteEditor *ui;
    DataManager *m_dataManager;
    QList<QLineEdit*> m_fieldInputs;
};