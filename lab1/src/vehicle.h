#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
using namespace std;


/**
 * @file vehicle.h
 * @brief Тип транспортного засобу.
 */

/**
 * @brief Перелік типів транспортних засобів.
 */
enum class VehicleType { Land, Water, Air };

/**
 * @brief Базовий клас для всіх транспортних засобів.
 *
 * Містить ім'я, тип та "вагу" (швидкість) транспортного засобу.
 * Використовується у графах для моделювання руху.
 */
class BaseVehicle {
protected:
    string name;        /**< Ім'я транспортного засобу */
    VehicleType type;   /**< Тип транспортного засобу (Land, Water, Air) */
    double weight;      /**< "Вага" транспортного засобу, чим менше — тим швидше */
public:

    /**
    * @brief Конструктор за замовчуванням
    */
    BaseVehicle() : name("Transport"), type(VehicleType::Land), weight(1.0) {}

    /**
     * @brief Конструктор з параметрами
     * @param n Ім'я транспортного засобу
     * @param t Тип транспортного засобу
     * @param w "Вага" транспортного засобу
     */
    BaseVehicle(string n, VehicleType t, double w) : name(n), type(t), weight(w) {}
    virtual ~BaseVehicle() = default;


    /**
     * @brief Повертає "вагу" транспортного засобу
     * @return weight
     */
    double getWeight() const { return weight; }


    /**
     * @brief Повертає ім'я транспортного засобу
     * @return name
     */
    string getName() const { return name; }

    /**
    * @brief Повертає тип транспортного засобу
    * @return type
    */
    VehicleType getType() const { return type; }

    /**
     * @brief Повертає рядкове представлення транспортного засобу
     * @return рядок виду "Name (Type, weight=W)"
     */
    string toString() const {
        string t = (type == VehicleType::Land ? "Land" :
                    type == VehicleType::Water ? "Water" : "Air");
        return name + " (" + t + ", weight=" + to_string(weight) + ")";
    }

    /**
     * @brief Перевантаження оператора == для порівняння двох транспортних засобів
     * @param other інший транспортний засіб
     * @return true, якщо рівні name, type та weight
     */
    bool operator==(const BaseVehicle& other) const {
        return name == other.name && type == other.type && weight == other.weight;
    }

    /**
     * @brief Перевантаження оператора < для сортування транспортних засобів
     * @param other інший транспортний засіб
     * @return true, якщо поточний об'єкт менший за other
     */
    bool operator<(const BaseVehicle& other) const {
        if (name != other.name) return name < other.name;
        if (type != other.type) return type < other.type;
        return weight < other.weight;
    }
};

/**
 * @brief Транспорт по суші
 */
class LandVehicle : public BaseVehicle {
public:
    LandVehicle() : BaseVehicle() {}
    LandVehicle(string n, double s) : BaseVehicle(n, VehicleType::Land, s) {}
};

/**
 * @brief Транспорт по воді
 */
class WaterVehicle : public BaseVehicle {
public:
    WaterVehicle() : BaseVehicle() {}
    WaterVehicle(string n, double s) : BaseVehicle(n, VehicleType::Water, s) {}
};

/**
 * @brief Повітряний транспорт
 */
class AirVehicle : public BaseVehicle {
public:
    AirVehicle() : BaseVehicle() {}
    AirVehicle(string n, double s) : BaseVehicle(n, VehicleType::Air, s) {}
};

/**
 * @brief Автомобіль
 */
class Car : public LandVehicle {
public:
    Car(std::string n = "Car", double s = 2.0) : LandVehicle(n, s) {}
};

/**
 * @brief Вантажівка
 */
class Truck : public LandVehicle {
public:
    Truck(std::string n = "Truck", double s = 1.5) : LandVehicle(n, s) {}
};

/**
 * @brief Парусна яхта
 */
class SailBoat : public WaterVehicle {
public:
    SailBoat(std::string n = "SailBoat", double s = 1.0) : WaterVehicle(n, s) {}
};

/**
 * @brief Моторний човен
 */
class MotorBoat : public WaterVehicle {
public:
    MotorBoat(std::string n = "MotorBoat", double s = 3.0) : WaterVehicle(n, s) {}
};

/**
 * @brief Літак
 */
class Plane : public AirVehicle {
public:
    Plane(std::string n = "Plane", double s = 5.0) : AirVehicle(n, s) {}
};

/**
 * @brief Гелікоптер
 */
class Helicopter : public AirVehicle {
public:
    Helicopter(std::string n = "Helicopter", double s = 4.0) : AirVehicle(n, s) {}
};

#endif
