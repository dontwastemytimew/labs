#pragma once
#include <QMainWindow>
#include "datamanager.h"
#include <QListWidgetItem>

namespace Ui { class MainWindow; }

/**
 * @class MainWindow
 * @brief Головне вікно програми.
 *
 * Відповідає за відображення головного інтерфейсу, списку нотаток та кнопок
 * для основних дій. Виступає як центральний вузол, що координує роботу
 * інших вікон та DataManager.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Конструктор головного вікна.
     * @param parent Вказівник на батьківський віджет (зазвичай nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief Оновлює список нотаток на екрані.
     *
     * Очищує список та заповнює його заново даними з DataManager,
     * враховуючи поточний текст у полі пошуку.
     */
    void updateNotesList();

    private slots:
    // --- Слоти для кнопок ---
    void on_manageSchemasButton_clicked();
    void on_createNoteButton_clicked();
    void deleteNoteLogic();
    void on_searchButton_clicked();
    void on_importNoteButton_clicked();
    void on_settingsButton_clicked();

    // --- Слоти для взаємодії зі списком нотаток ---
    /**
     * @brief Викликається при подвійному кліку на елемент списку нотаток.
     * @param item Елемент, на який було здійснено клік.
     */
    void onNoteDoubleClicked(QListWidgetItem *item);

    /**
     * @brief Викликається при кліку правою кнопкою миші на списку нотаток.
     * @param pos Позиція курсора.
     */
    void onNotesListContextMenuRequested(const QPoint &pos);


protected:
    /**
     * @brief Перехоплює подію закриття вікна для збереження даних.
     * @param event Подія закриття.
     */
    void closeEvent(QCloseEvent *event) override;

 void changeEvent(QEvent *event) override;


private:
 Ui::MainWindow *ui; ///< Вказівник на об'єкти, створені у Qt Designer.
 DataManager m_dataManager; ///< Екземпляр "мозку" програми, що зберігає всі дані.
};