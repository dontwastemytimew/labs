#pragma once
#include <QDialog>
#include <QSet>

namespace Ui { class TagEditor; }

/**
 * @class TagEditor
 * @brief Діалогове вікно для керування тегами однієї нотатки.
 *
 * Надає користувачеві інтерфейс для перегляду списку існуючих тегів
 * та додавання нових.
 */
class TagEditor : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор вікна редактора тегів.
     * @param currentTags Набір існуючих тегів, які потрібно відобразити.
     * @param parent Вказівник на батьківський віджет.
     */
    explicit TagEditor(const QSet<QString>& currentTags, QWidget *parent = nullptr);
    ~TagEditor();

    /**
     * @brief Повертає оновлений набір тегів після редагування.
     * @return QSet<QString> з фінальним набором тегів.
     */
    QSet<QString> getTags() const;

    private slots:
    /**
     * @brief Слот, що викликається при натисканні на кнопку "Додати тег".
     */
        void on_addTagButton_clicked();

private:
    Ui::TagEditor *ui; ///< Вказівник на об'єкти інтерфейсу.
    QSet<QString> m_tags; ///< Тимчасове сховище для набору тегів.
};