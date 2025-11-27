#pragma once
#include <QWidget>
#include "datamanager.h"
#include <QDialog>

namespace Ui { class SchemaManager; }

/**
 * @class SchemaManager
 * @brief Діалогове вікно для керування схемами нотаток.
 *
 * Надає користувачеві інтерфейс для перегляду списку існуючих схем,
 * а також для їх створення, редагування та видалення.
 */
class SchemaManager : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор вікна керування схемами.
     * @param dataManager Вказівник на центральне сховище даних.
     * @param parent Вказівник на батьківський віджет.
     */
    explicit SchemaManager(DataManager *dataManager, QWidget *parent = nullptr);
    ~SchemaManager();

    private slots:
    /**
     * @brief Слот, що викликається при натисканні на кнопку "Створити".
     *
     * Відкриває SchemaEditor для створення нової схеми.
     */
    void openSchemaEditor();

    /**
     * @brief Слот, що викликається при натисканні на кнопку "Видалити".
     *
     * Видаляє обрану у списку схему.
     */
    void on_deleteSchemaButton_clicked();

    /**
     * @brief Слот, що викликається при натисканні на кнопку "Редагувати".
     *
     * Відкриває SchemaEditor з даними обраної у списку схеми.
     */
    void on_editSchemaButton_clicked();

 /**
     * @brief Слот, що викликається при натисканні на кнопку "Закрити".
     *
     * Закриває поточне діалогове вікно.
     */
    void on_closeButton_clicked();

 protected:
 void changeEvent(QEvent *event) override;

private:
 /**
     * @brief Оновлює список схем на екрані.
     */
    void updateSchemasList();

 Ui::SchemaManager *ui;      ///< Вказівник на об'єкти інтерфейсу.
 DataManager *m_dataManager; ///< Вказівник на "мозок" програми.
};
