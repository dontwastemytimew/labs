#ifndef INTEGRATION_TESTS_H
#define INTEGRATION_TESTS_H

#include <iostream>
#include <cassert>
#include "environment.h"
#include "adjlist.h"
#include "vehicle.h"


class IntegrationTests {
private:
    static void testGraphAndVehiclesFromFile() {
        Environment<AdjacencyListGraph<std::string>, std::string> env;
        // Зчитування
        env.loadGraphFromFile("graph.csv");
        env.loadVehiclesFromFile("vehicles.csv");

        assert(!env.graphToString().empty() && "Graph should be loaded from file");
        std::cout << "Graph loaded from file:\n" << env.graphToString() << "\n";
        std::cout << "Vehicles loaded from file:\n";
        env.showVehicles();

        // do something
        // Car -> A->B
        env.simulateRoute(0, {"A","B"});
        assert(env.canMove(0,"A","B") && "Car should be able to move A->B");

        // MotorBoat -> B->C
        env.simulateRoute(1, {"B","C"});
        assert(env.canMove(1,"B","C") && "MotorBoat should be able to move B->C");

        // Plane -> A->F
        env.simulateRoute(2, {"A","F"});
        assert(env.canMove(2,"A","F") && "Plane should be able to move A->F");


        std::cout << "Vehicle routes simulated successfully.\n";

        // Збереження
        env.saveGraphToFile("graph_out.csv");
        env.saveVehiclesToFile("vehicles_out.csv");

        std::cout << "Graph and vehicles saved to graph_out.csv and vehicles_out.csv\n";
    }

public:
    static void runAllTests() {
        testGraphAndVehiclesFromFile();
        std::cout << "\nIntegration tests passed!\n";
    }
};


#endif //INTEGRATION_TESTS_H
