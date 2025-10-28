#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schemamanager.h"
#include "noteeditor.h"
#include <QDebug>
#include <QMenu>
#include <QListWidgetItem>
#include "notewidget.h"
#include "tageditor.h"
#include <QFileDialog>
#include "datamanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->notesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->notesListWidget, &QWidget::customContextMenuRequested, this, &MainWindow::onNotesListContextMenuRequested);
    connect(ui->notesListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onNoteDoubleClicked);

    m_dataManager.loadFromFile("data.json");

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_manageSchemasButton_clicked()
{
    SchemaManager schemaManagerWindow(&m_dataManager, this);

    schemaManagerWindow.exec();
}

void MainWindow::updateNotesList()
{
    QString searchText = ui->searchLineEdit->text();
    ui->notesListWidget->clear();

    for (const auto& note : m_dataManager.getNotes()) {
        if (searchText.isEmpty() || note.getTitle().contains(searchText, Qt::CaseInsensitive))
        {
            auto item = new QListWidgetItem();

            auto noteWidget = new NoteWidget(note);

            item->setBackground(QColor("white"));


            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

            item->setText("");

            item->setSizeHint(noteWidget->sizeHint());

            ui->notesListWidget->addItem(item);

            ui->notesListWidget->setItemWidget(item, noteWidget);
        }
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

void MainWindow::closeEvent(QCloseEvent *event) {
    m_dataManager.saveToFile("data.json");
    QMainWindow::closeEvent(event);
}

void MainWindow::deleteNoteLogic()
{

    int currentIndex = ui->notesListWidget->currentRow();

    if (currentIndex < 0) {
        return;
    }

    m_dataManager.removeNote(currentIndex);

    updateNotesList();
}

void MainWindow::onNotesListContextMenuRequested(const QPoint &pos)
{

    QListWidgetItem* item = ui->notesListWidget->itemAt(pos);
    if (!item) {
        return;
    }

    int currentIndex = ui->notesListWidget->row(item);

    QMenu contextMenu(this);

    QAction *editTagsAction = contextMenu.addAction("Редагувати теги");
    QAction *exportAction = contextMenu.addAction("Експортувати (.json)");
    QAction *deleteAction = contextMenu.addAction("Видалити нотатку");

    QAction *selectedAction = contextMenu.exec(ui->notesListWidget->mapToGlobal(pos));

    if (selectedAction == deleteAction) {
        deleteNoteLogic();

    } else if (selectedAction == editTagsAction) {
        Note& noteToEdit = m_dataManager.getNotes()[currentIndex];

        TagEditor editor(noteToEdit.getTags(), this);
        if (editor.exec() == QDialog::Accepted) {
            noteToEdit.setTags(editor.getTags());
            m_dataManager.updateNote(currentIndex, noteToEdit);
            updateNotesList();
        }

    } else if (selectedAction == exportAction) {


        QString defaultFileName = m_dataManager.getNotes()[currentIndex].getTitle();

        QString fileName = QFileDialog::getSaveFileName(this,
            "Експортувати нотатку",
            defaultFileName + ".json",
            "JSON Files (*.json)");

        if (!fileName.isEmpty()) {
            m_dataManager.exportNote(currentIndex, fileName);
        }
    }
}


void MainWindow::onNoteDoubleClicked(QListWidgetItem *item)
{
    int currentIndex = ui->notesListWidget->row(item);
    if (currentIndex < 0) return;

    const Note& noteToEdit = m_dataManager.getNotes()[currentIndex];

    NoteEditor editor(&m_dataManager, noteToEdit, this);

    if (editor.exec() == QDialog::Accepted)
    {
        Note updatedNote = editor.getNote();
        m_dataManager.updateNote(currentIndex, updatedNote);
        updateNotesList();
    }
}

void MainWindow::on_searchButton_clicked()
{

    updateNotesList();
}

void MainWindow::on_importNoteButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Імпортувати нотатку",
        QDir::homePath(),
        "JSON Files (*.json)");

    if (!fileName.isEmpty()) {
        m_dataManager.importNote(fileName);


        updateNotesList();


    }
}