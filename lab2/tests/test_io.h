#ifndef TEST_IO_H
#define TEST_IO_H

#include <cassert>
#include <iostream>
#include <QFile>
#include "../datamanager.h"

class TestIO {
private:
    static void test_save_and_load_data() {
        std::cout << "  - Testing Save/Load functionality... ";

        const QString testPath = "test_data_temp.json";

        DataManager dm_save;
        Schema s("Схема для тесту");
        s.addField({"Розділ", "Текст"});
        dm_save.addSchema(s);

        Note n("Збережена нотатка", 1); // 1, бо 0 - це дефолтна схема
        n.addTag("test_tag");
        n.addField("Розділ", "Тестовий вміст");
        dm_save.addNote(n);

        dm_save.saveToFile(testPath);

        DataManager dm_load;
        dm_load.loadFromFile(testPath);

        // Перевірка кількості
        assert(dm_load.getSchemas().size() == dm_save.getSchemas().size());
        assert(dm_load.getNotes().size() == dm_save.getNotes().size());

        // Перевірка вмісту нотатки
        const Note& loadedNote = dm_load.getNotes()[0];
        assert(loadedNote.getTitle() == "Збережена нотатка");
        assert(loadedNote.getTags().contains("test_tag"));
        assert(loadedNote.getFields().value("Розділ") == "Тестовий вміст");

        QFile::remove(testPath);

        std::cout << "Passed!\n";
    }

    static void test_single_note_io() {
        std::cout << "  - Testing single Note Export/Import... ";

        const QString testPath = "test_note_temp.json";

        DataManager dm_export;
        Note n_out("Експортована нотатка", 0);
        n_out.addTag("експорт");
        n_out.addField("Поле", "Значення");
        dm_export.addNote(n_out);

        dm_export.exportNote(0, testPath);

        DataManager dm_import;
        dm_import.importNote(testPath);

        assert(dm_import.getNotes().size() == 1);

        const Note& n_in = dm_import.getNotes()[0];
        assert(n_in.getTitle() == "Експортована нотатка");
        assert(n_in.getTags().contains("експорт"));
        assert(n_in.getFields().value("Поле") == "Значення");

        QFile::remove(testPath);

        std::cout << "Passed!\n";
    }

    static void test_load_corrupted_file() {
        std::cout << "  - Testing corrupted file load... ";

        const QString testPath = "corrupted_temp.json";

        QFile file(testPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write("Це не JSON!");
            file.close();
        }

        DataManager dm;
        dm.loadFromFile(testPath);

        assert(dm.getSchemas().size() == 1);
        assert(dm.getNotes().isEmpty());

        QFile::remove(testPath);
        std::cout << "Passed!\n";
    }

public:
    static void runTests() {
        std::cout << "--- Running I/O Tests ---\n";
        test_save_and_load_data();
        test_single_note_io();
        test_load_corrupted_file();
        std::cout << "--- I/O Tests Passed ---\n";
    }
};

#endif //TEST_IO_H
