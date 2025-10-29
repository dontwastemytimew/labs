#ifndef TEST_DATAMANAGER_H
#define TEST_DATAMANAGER_H

#include <cassert>
#include <iostream>
#include "../datamanager.h"

class TestDataManager {
private:
    static void test_manager_crud() {
        std::cout << "  - Testing CRUD operations... ";
        DataManager dm;

        // Створюємо базові елементи
        Schema s1("Схема A");
        dm.addSchema(s1);
        Note n1("Нотатка 1", 0);
        dm.addNote(n1);

        // Читання/Кількість
        assert(dm.getSchemas().size() > 0);
        assert(dm.getNotes().size() == 1);

        // Оновлення нотатки
        Note n2("Нова назва", 0);
        dm.updateNote(0, n2);
        assert(dm.getNotes()[0].getTitle() == "Нова назва");

        // Оновлення схеми
        Schema s_upd("Схема Update");
        dm.updateSchema(0, s_upd);
        assert(dm.getSchemas()[0].getName() == "Схема Update");

        // Видалення (Delete)
        dm.removeNote(0);
        dm.removeSchema(0);
        assert(dm.getNotes().isEmpty());

        std::cout << "Passed!\n";
    }

    // Перевірка, що DataManager повертає посилання, яке можна змінювати
    static void test_mutable_notes() {
        std::cout << "  - Testing mutable access to Notes... ";
        DataManager dm;
        Note n1("Temp Note", 0);
        dm.addNote(n1);

        // Отримуємо посилання на нотатку
        dm.getNotes()[0].setTitle("Змінено через посилання");

        assert(dm.getNotes()[0].getTitle() == "Змінено через посилання");
        std::cout << "Passed!\n";
    }

    static void test_manager_constructor() {
        std::cout << "  - Testing DataManager constructor default schema... ";
        DataManager dm;
        assert(dm.getSchemas().size() == 1); // Перевіряємо, що одна схема існує
        assert(dm.getSchemas()[0].getName() == "Книга");
        assert(dm.getSchemas()[0].getFields().size() == 2); // Перевіряємо, що в неї 2 поля
        std::cout << "Passed!\n";
    }

    static void test_invalid_indices() {
        std::cout << "  - Testing invalid index handling... ";
        DataManager dm;
        Note n("Test", 0);
        dm.addNote(n);

        // Спробуємо видалити з недійсними індексами
        dm.removeNote(-1);
        dm.removeNote(99);

        // Спробуємо оновити з недійсними індексами
        dm.updateNote(-1, n);
        dm.updateNote(99, n);

        // Якщо програма не "впала" і нотатка все ще на місці, тест пройдено
        assert(dm.getNotes().size() == 1);

        std::cout << "Passed!\n";
    }

public:
    static void runTests() {
        std::cout << "--- Running DataManager Tests ---\n";
        test_manager_crud();
        test_mutable_notes();
        test_manager_constructor();
        test_invalid_indices();
        std::cout << "--- DataManager Tests Passed ---\n";
    }
};

#endif //TEST_DATAMANAGER_H
