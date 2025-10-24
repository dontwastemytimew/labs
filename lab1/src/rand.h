#ifndef RAND_H
#define RAND_H

#include <type_traits>
#include <cstdlib>
#include <string>
#include <vector>
#include "vehicle.h"


/**
 * @file rand.h
 * @brief Дозволяє швидко генерувати випадкові значення для графів та транспортних об'єктів.
 *
 * Шаблони та спеціалізації для числових типів, рядків, векторів та об'єктів BaseVehicle.
 */

/**
 * @brief Загальний шаблон для генерації випадкового значення типу T.
 *
 * Для арифметичних типів повертає випадкове число від 0 до maxValue.
 * Для інших типів повертає "пустий" об'єкт T.
 *
 * @tparam T Тип значення.
 * @param maxValue Максимальне значення (для арифметичних типів).
 * @return Випадкове значення типу T.
 */
template <typename T>
inline T generateRandomEdge(const T& maxValue) {
    if (std::is_arithmetic<T>::value) {
        return rand() % (static_cast<int>(maxValue) + 1);
    } else {
        return T{}; ///< пустий об'єкт для всіх інших класів
    }
}

/**
 * @brief Спеціалізація шаблону для BaseVehicle.
 *
 * Генерує випадковий транспорт з випадковим ім'ям, типом та швидкістю.
 *
 * @param dummy Dummy-аргумент для спеціалізації.
 * @return BaseVehicle Випадковий транспортний засіб.
 */
template <>
inline BaseVehicle generateRandomEdge<BaseVehicle>(const BaseVehicle&) {
    std::string names[] = {"Car", "Truck", "Boat", "Plane", "Bus"};
    double speed = 1.0 + rand() % 100 / 10.0; // швидкість від 1.0 до 10.0
    int idx = rand() % 5;
    VehicleType type = static_cast<VehicleType>(rand() % 3);
    return BaseVehicle(names[idx], type, speed);
}

/**
 * @brief Спеціалізація шаблону для std::string.
 *
 * Генерує випадковий рядок довжиною від 3 до 7 символів.
 *
 * @param dummy Dummy-аргумент для спеціалізації.
 * @return std::string Випадковий рядок.
 */
template <>
inline std::string generateRandomEdge<std::string>(const std::string&) {
    std::string s;
    int len = 3 + rand() % 5; // довжина від 3 до 7
    for (int i = 0; i < len; i++)
        s += 'A' + rand() % 26;
    return s;
}


/**
 * @brief Спеціалізація шаблону для std::vector<int>.
 *
 * Генерує вектор випадкових цілих чисел довжиною від 1 до 5.
 *
 * @param dummy Dummy-аргумент для спеціалізації.
 * @return std::vector<int> Випадковий вектор цілих чисел.
 */
template <>
inline std::vector<int> generateRandomEdge<std::vector<int>>(const std::vector<int>&) {
    int len = 1 + rand() % 5;
    std::vector<int> v(len);
    for (int i = 0; i < len; i++)
        v[i] = rand() % 100;
    return v;
}


#endif //RAND_H
