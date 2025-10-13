#ifndef VEHICLE_GRAPH_TESTS_H
#define VEHICLE_GRAPH_TESTS_H

#include <cassert>
#include <iostream>
#include "adjlist.h"
#include "vehicle.h"


class VehicleGraphTests {
private:
    static void test_vehicleGraph() {
        AdjacencyListGraph<BaseVehicle> g(false);

        Car car("Car", 2.0);
        SailBoat boat("Boat", 1.0);

        g.addVertex(car);
        g.addVertex(boat);

        g.addEdge(car, boat, car.getWeight() + boat.getWeight());

        assert(g.distance(car, boat) == car.getWeight() + boat.getWeight());
        assert(g.isConnected());
        assert(g.distance(boat, car) == car.getWeight() + boat.getWeight());

        std::cout << "Vehicle-Graph integration test passed.\n";
    }


public:
    static void runAllTests() {
        test_vehicleGraph();
    }
};


#endif // VEHICLE_GRAPH_TESTS_H
