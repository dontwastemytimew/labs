#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schemamanager.h"
#include "noteeditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_manageSchemasButton_clicked()
{
    auto schemaManagerWindow = new SchemaManager(&m_dataManager, this);
    schemaManagerWindow->show();
}

void MainWindow::updateNotesList()
{
    ui->notesListWidget->clear();
    for (const auto& note : m_dataManager.getNotes()) {
        ui->notesListWidget->addItem(note.getTitle());
    }
}

void MainWindow::on_createNoteButton_clicked()
{
    NoteEditor editor(&m_dataManager, this);

    if (editor.exec() == QDialog::Accepted)
    {
        Note newNote = editor.getNote();

        m_dataManager.addNote(newNote);

        updateNotesList();
    }
}