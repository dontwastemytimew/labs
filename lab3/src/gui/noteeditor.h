#pragma once
#include <QDialog>
#include "datamanager.h"
#include <QLineEdit>
#include "note.h"


namespace Ui { class NoteEditor; }

/**
 * @class NoteEditor
 * @brief Діалогове вікно для створення та редагування однієї нотатки.
 *
 * "Розумний" редактор, який динамічно генерує поля для вводу на основі
 * обраної користувачем схеми.
 */
class NoteEditor : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор для створення нової нотатки.
     * @param dataManager Вказівник на центральне сховище даних (потрібен для отримання списку схем).
     * @param parent Вказівник на батьківський віджет.
     */
    explicit NoteEditor(DataManager *dataManager, QWidget *parent = nullptr);

    /**
     * @brief Конструктор для редагування існуючої нотатки.
     * @param dataManager Вказівник на центральне сховище даних.
     * @param noteToEdit Нотатка, дані якої потрібно завантажити в редактор.
     * @param parent Вказівник на батьківський віджет.
     */
    explicit NoteEditor(DataManager *dataManager, const Note& noteToEdit, QWidget *parent = nullptr);

    ~NoteEditor();

    /**
     * @brief Збирає дані з усіх полів вводу та повертає готовий об'єкт Note.
     * @return Створений або оновлений об'єкт Note.
     */
    Note getNote() const;

    private slots:
 /**
     * @brief Слот, що викликається при зміні обраної схеми у випадаючому списку.
     *
     * Динамічно генерує поля для вводу відповідно до обраної схеми.
     * @param index Індекс обраної схеми.
     */
    void onSchemaSelected(int index);

    /**
     * @brief Перевизначений слот для валідації даних перед закриттям вікна.
     *
     * Перевіряє, що назва нотатки не є порожньою.
     */
    void accept() override;

 void on_addImageButton_clicked();

 protected:
 void changeEvent(QEvent *event) override;

private:
 Ui::NoteEditor *ui;             ///< Вказівник на об'єкти інтерфейсу.
 DataManager *m_dataManager;     ///< Вказівник на "мозок" програми.
 QList<QLineEdit*> m_fieldInputs;///< Тимчасове сховище для вказівників на динамічно створені поля вводу.
 QSet<QString> m_originalTags;   ///< Тимчасове сховище для тегів нотатки під час її редагування.

 QString m_currentImageBase64;


 void displayImage(const QString& base64);
};