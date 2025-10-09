#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
using namespace std;

// Класи дозволяють створювати транспорт різного типу.
// getWeight() інтегрується з графами для пошуку оптимальних маршрутів.
// toString() дозволяє зручно виводити інформацію про транспорт.

enum class VehicleType { Land, Water, Air };

// Базовий клас транспорту
class BaseVehicle {
protected:
    string name;
    VehicleType type;
    double weight;  // чим швидше, тим менша "вага"
public:
    BaseVehicle() : name("Transport"), type(VehicleType::Land), weight(1.0) {}
    BaseVehicle(string n, VehicleType t, double w) : name(n), type(t), weight(w) {}
    virtual ~BaseVehicle() = default;

    double getWeight() const { return weight; }
    string getName() const { return name; }
    VehicleType getType() const { return type; }

    string toString() const {
        string t = (type == VehicleType::Land ? "Land" :
                    type == VehicleType::Water ? "Water" : "Air");
        return name + " (" + t + ", weight=" + std::to_string(weight) + ")";
    }

    // Це перевантаження оператора == для класу BaseVehicle, порівнювати два об’єкти BaseVehicle на рівність
    bool operator==(const BaseVehicle& other) const {
        return name == other.name && type == other.type && weight == other.weight;
    }

    // Це перевантаження оператора <, дозволити сортування об’єктів BaseVehicle
    bool operator<(const BaseVehicle& other) const {
        if (name != other.name) return name < other.name;
        if (type != other.type) return type < other.type;
        return weight < other.weight;
    }
};

// Конкретні класи
class LandVehicle : public BaseVehicle {
public:
    LandVehicle() : BaseVehicle() {}
    LandVehicle(string n, double s) : BaseVehicle(n, VehicleType::Land, s) {}
};

class WaterVehicle : public BaseVehicle {
public:
    WaterVehicle() : BaseVehicle() {}
    WaterVehicle(string n, double s) : BaseVehicle(n, VehicleType::Water, s) {}
};

class AirVehicle : public BaseVehicle {
public:
    AirVehicle() : BaseVehicle() {}
    AirVehicle(string n, double s) : BaseVehicle(n, VehicleType::Air, s) {}
};

#endif
