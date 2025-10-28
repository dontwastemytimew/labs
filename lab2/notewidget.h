#pragma once
#include <QWidget>
#include "note.h"

namespace Ui { class NoteWidget; }

class NoteWidget : public QWidget {
    Q_OBJECT

public:
    explicit NoteWidget(const Note& note, QWidget *parent = nullptr);
    ~NoteWidget();

private:
    Ui::NoteWidget *ui;
};