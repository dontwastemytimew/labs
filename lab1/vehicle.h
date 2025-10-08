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
    double speed;  // чим швидше, тим менша "вага"
public:
    BaseVehicle() : name("Transport"), type(VehicleType::Land), speed(1.0) {} // <-- Дефолтний конструктор
    BaseVehicle(string n, VehicleType t, double s) : name(n), type(t), speed(s) {}
    virtual ~BaseVehicle() = default;

    double getWeight() const { return 1.0 / speed; }  // для графа (1/speed)
    string getName() const { return name; }
    VehicleType getType() const { return type; }

    string toString() const {    // текстове представлення
        string t = (type == VehicleType::Land ? "Land" :
                    type == VehicleType::Water ? "Water" : "Air");
        return name + " (" + t + ", speed=" + std::to_string(speed) + ")";
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
