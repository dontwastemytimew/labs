#ifndef VEHICLE_TESTS_H
#define VEHICLE_TESTS_H

#include <cassert>
#include "vehicle.h"
#include <string>


class VehicleTests {
private:
    static void test_constructorAndGetters() {
        Car car("Car", 10.0);
        assert(car.getName() == "Car");
        assert(car.getType() == VehicleType::Land);
        assert(car.getWeight() == 10.0);
    }

    static void test_toString() {
        Plane plane("Plane", 2.0);
        std::string str = plane.toString();
        assert(str.find("Plane") != std::string::npos);
        assert(str.find("Air") != std::string::npos);
    }

    static void test_comparisonOperators() {
        Car car1("Car", 10.0);
        Car car2("Car", 10.0);
        Car car3("Car", 5.0);
        assert(car1 == car2);
        assert(!(car1 == car3));
        assert(car3 < car1);
    }

    static void test_vehicleTypes() {
        Car car; Plane plane; SailBoat boat;
        assert(car.getType() == VehicleType::Land);
        assert(plane.getType() == VehicleType::Air);
        assert(boat.getType() == VehicleType::Water);
    }

public:
    static void runAllTests() {
        test_constructorAndGetters();
        test_toString();
        test_comparisonOperators();
        test_vehicleTypes();
    }
};


#endif //VEHICLE_TESTS_H
