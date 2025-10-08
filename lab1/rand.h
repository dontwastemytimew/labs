#ifndef RAND_H
#define RAND_H


#include <type_traits>
#include <cstdlib>
#include <string>
#include "vehicle.h"

// Дозволяє швидко створювати випадкові ребра для графа.
// Для BaseVehicle генерує випадковий транспорт з ім’ям, типом та швидкістю.

// Загальний шаблон
template <typename T>
T generateRandomEdge(const T& maxValue) {
    if (std::is_arithmetic<T>::value) {
        return rand() % (static_cast<int>(maxValue) + 1);
    } else {
        return T{}; // пустий об'єкт для всіх інших класів
    }
}

// Спеціалізація для BaseVehicle
template <>
BaseVehicle generateRandomEdge<BaseVehicle>(const BaseVehicle&) {
    std::string names[] = {"Car", "Truck", "Boat", "Plane", "Bus"};
    double speed = 1.0 + rand() % 100 / 10.0; // швидкість від 1.0 до 10.0
    int idx = rand() % 5;
    VehicleType type = static_cast<VehicleType>(rand() % 3);
    return BaseVehicle(names[idx], type, speed);
}

// Спеціалізація для string
template <>
std::string generateRandomEdge<std::string>(const std::string&) {
    std::string s;
    int len = 3 + rand() % 5; // довжина від 3 до 7
    for (int i = 0; i < len; i++)
        s += 'A' + rand() % 26;
    return s;
}

// Спеціалізація для vector<int>
template <>
std::vector<int> generateRandomEdge<std::vector<int>>(const std::vector<int>&) {
    int len = 1 + rand() % 5;
    std::vector<int> v(len);
    for (int i = 0; i < len; i++)
        v[i] = rand() % 100;
    return v;
}


#endif //RAND_H
