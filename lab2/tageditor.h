#pragma once
#include <QDialog>
#include <QSet>

namespace Ui { class TagEditor; }

class TagEditor : public QDialog {
    Q_OBJECT

public:
    explicit TagEditor(const QSet<QString>& currentTags, QWidget *parent = nullptr);
    ~TagEditor();

    QSet<QString> getTags() const;

    private slots:
        void on_addTagButton_clicked();

private:
    Ui::TagEditor *ui;
    QSet<QString> m_tags;
};