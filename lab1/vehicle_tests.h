#ifndef VEHICLE_TESTS_H
#define VEHICLE_TESTS_H

#include <cassert>
#include "vehicle.h"
#include <string>

class VehicleTests {
private:
    static void test_constructorAndGetters() {
        LandVehicle lv("Car", 10.0);
        assert(lv.getName() == "Car");
        assert(lv.getType() == VehicleType::Land);
        assert(lv.getWeight() == 10.0);
    }

    static void test_toString() {
        AirVehicle av("Plane", 2.0);
        std::string str = av.toString();
        assert(str.find("Plane") != std::string::npos);
        assert(str.find("Air") != std::string::npos);
    }

public:
    static void runAllTests() {
        test_constructorAndGetters();
        test_toString();
    }
};

#endif //VEHICLE_TESTS_H
