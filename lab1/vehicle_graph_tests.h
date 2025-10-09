#ifndef VEHICLE_GRAPH_TESTS_H
#define VEHICLE_GRAPH_TESTS_H

#include <cassert>
#include <iostream>
#include "adjlist.h"
#include "vehicle.h"

class VehicleGraphTests {
private:
    static void test_vehicleIntegrationWithGraph() {
        AdjacencyListGraph<BaseVehicle> g(false);

        LandVehicle car("Car", 10.0);      // weight = 1/10 = 0.1
        AirVehicle plane("Plane", 2.0);    // weight = 1/2 = 0.5
        WaterVehicle boat("Boat", 5.0);    // weight = 1/5 = 0.2

        g.addVertex(car);
        g.addVertex(plane);
        g.addVertex(boat);

        // Додаємо ребра між транспортними засобами, використовуючи вагу
        g.addEdge(car, plane, car.getWeight() + plane.getWeight());   // 0.1 + 0.5 = 0.6
        g.addEdge(plane, boat, plane.getWeight() + boat.getWeight()); // 0.5 + 0.2 = 0.7

        assert(g.distance(car, plane) == car.getWeight() + plane.getWeight());
        assert(g.distance(plane, boat) == plane.getWeight() + boat.getWeight());

        assert(g.isConnected());

        std::cout << "Vehicle-Graph integration test passed.\n";
    }

public:
    static void runAllTests() {
        test_vehicleIntegrationWithGraph();
    }
};

#endif // VEHICLE_GRAPH_TESTS_H
