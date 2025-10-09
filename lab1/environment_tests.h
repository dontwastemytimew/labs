#ifndef ENVIRONMENT_TESTS_H
#define ENVIRONMENT_TESTS_H

#include <cassert>
#include <iostream>
#include "environment.h"
#include "adjlist.h"
#include "vehicle.h"

class EnvironmentTests {
private:
    static void test_environment() {

        AdjacencyListGraph<std::string> g(true);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");

        g.addEdge("A","B",5.0);
        g.addEdge("B","C",3.0);
        g.addEdge("A","C",8.0);

        Environment<AdjacencyListGraph<std::string>, std::string> env(g);

        LandVehicle car("Car", 2.0);
        WaterVehicle boat("Boat", 1.0);
        AirVehicle plane("Plane", 5.0);

        env.addVehicle(car, "A");
        env.addVehicle(boat, "B");
        env.addVehicle(plane, "A");

        env.addPath("A","B",5.0, VehicleType::Land);
        env.addPath("B","C",3.0, VehicleType::Water);
        env.addPath("A","C",8.0, VehicleType::Air);

        assert(env.canMove(0,"A","B") && "Car should be able to move A->B");
        assert(!env.canMove(0,"B","A") && "Car should not be able to move B->A");
        assert(env.canMove(1,"B","C") && "Boat should be able to move B->C");
        assert(!env.canMove(1,"C","B") && "Boat should not be able to move C->B");
        assert(env.canMove(2,"A","C") && "Plane should be able to move A->C (any road type)");
        assert(!env.canMove(2,"C","B") && "Plane cannot move C->B if edge doesn't exist");

    }

public:
    static void runAllTests() {
        test_environment();
    }
};

#endif //ENVIRONMENT_TESTS_H
