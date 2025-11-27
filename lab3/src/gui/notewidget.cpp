#include "notewidget.h"
#include "ui_notewidget.h"

NoteWidget::NoteWidget(const Note &note, QWidget *parent) :
    QWidget(parent), ui(new Ui::NoteWidget)
{
    ui->setupUi(this);

    ui->titleLabel->setText(note.getTitle());
    ui->dateLabel->setText(note.getCreationDate().toString("dd.MM.yyyy"));

    const QSet<QString>& tags = note.getTags();

    if (!tags.isEmpty()) {
        QStringList tagsList = tags.values();
        ui->tagsLabel->setText(tr("#") + tagsList.join(tr(" #")));
    } else {
        ui->tagsLabel->hide();
    }

    QString base64Image = note.getImage();
    if (!base64Image.isEmpty()) {
        QByteArray imageData = QByteArray::fromBase64(base64Image.toLatin1());
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            ui->thumbLabel->setPixmap(pixmap.scaled(60, 60,
                                      Qt::KeepAspectRatioByExpanding,
                                      Qt::SmoothTransformation));
        }
        ui->thumbLabel->show();
    } else {
        ui->thumbLabel->hide();
    }
}

NoteWidget::~NoteWidget() {
    delete ui;
}