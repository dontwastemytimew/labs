#include "notewidget.h"
#include "ui_notewidget.h"

NoteWidget::NoteWidget(const Note &note, QWidget *parent) :
    QWidget(parent), ui(new Ui::NoteWidget)
{
    ui->setupUi(this);

    ui->titleLabel->setStyleSheet("QLabel { color: black; }");
    ui->dateLabel->setStyleSheet("QLabel { color: #555555; }");
    ui->tagsLabel->setStyleSheet("QLabel { color: #777777; }");

    ui->titleLabel->setText(note.getTitle());
    ui->dateLabel->setText(note.getCreationDate().toString("dd.MM.yyyy"));

    const QSet<QString>& tags = note.getTags();

    if (!tags.isEmpty()) {
        QStringList tagsList = tags.values();
        ui->tagsLabel->setText(tr("#") + tagsList.join(tr(" #")));
    } else {
        ui->tagsLabel->hide();
    }
}

NoteWidget::~NoteWidget() {
    delete ui;
}