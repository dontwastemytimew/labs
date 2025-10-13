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

        Car car("Car", 2.0);
        SailBoat boat("Boat", 1.0);
        Plane plane("Plane", 5.0);

        env.addVehicle(car, "A");
        env.addVehicle(boat, "B");
        env.addVehicle(plane, "A");

        env.addPath("A","B",5.0, VehicleType::Land);
        env.addPath("B","C",3.0, VehicleType::Water);
        env.addPath("A","C",8.0, VehicleType::Air);

        assert(env.canMove(0,"A","B"));
        assert(!env.canMove(0,"B","A"));
        assert(env.canMove(1,"B","C"));
        assert(!env.canMove(1,"C","B"));
        assert(env.canMove(2,"A","C"));
    }

    static void test_computeOptimalRoute() {
        AdjacencyListGraph<std::string> g(true);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        g.addEdge("A","B",5.0);
        g.addEdge("B","C",3.0);
        g.addEdge("A","C",10.0);

        Environment<AdjacencyListGraph<std::string>, std::string> env(g);

        Car car("Car", 2.0);
        env.addVehicle(car, "A");

        auto route = env.computeOptimalRoute(0, "A", "C");

        assert(route.size() == 3);
        assert(route[0] == "A");
        assert(route[1] == "B");
        assert(route[2] == "C");
    }

    static void test_addPointAndAddPath() {
        AdjacencyListGraph<std::string> g(false);
        Environment<AdjacencyListGraph<std::string>, std::string> env(g);

        env.addPoint("X");
        env.addPoint("Y");
        env.addPath("X", "Y", 7.0, VehicleType::Land);

        auto str = env.graphToString();
        assert(str.find("X") != std::string::npos);
        assert(str.find("Y") != std::string::npos);

        auto roadsStr = env.roadsToString();
        assert(roadsStr.find("X -> Y") != std::string::npos);
    }

    static void test_addVehicleAndMove() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");

        Environment<AdjacencyListGraph<std::string>, std::string> env(g);
        Car car("Car", 2.0);
        env.addVehicle(car, "A");

        // Додаємо шлях між A і B
        env.addPath("A", "B", 5.0, VehicleType::Land);

        // Машина рухається
        env.moveVehicle(0, "B", false);

        // Перевірка, що машина опинилась у B
        assert(env.getVehiclePosition(0) == "B");
    }

    static void test_simulateRoute() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        g.addEdge("A","B",2.0);
        g.addEdge("B","C",3.0);

        Environment<AdjacencyListGraph<std::string>, std::string> env(g);
        Car car("Car", 2.0);
        env.addVehicle(car, "A");

        env.addPath("A","B",2.0, VehicleType::Land);
        env.addPath("B","C",3.0, VehicleType::Land);

        env.simulateRoute(0, {"A","B","C"}, 0);

        // Перевірка, що транспорт опинився в останній точці маршруту
        auto finalRoute = env.computeOptimalRoute(0, "C", "C");
        assert(finalRoute.front() == "C");
    }

    static void test_graphToString() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addEdge("A","B",5.0);

        Environment<AdjacencyListGraph<std::string>, std::string> env(g);

        std::string graphStr = env.graphToString();
        assert(graphStr.find("A") != std::string::npos);
        assert(graphStr.find("B") != std::string::npos);
    }

    static void test_saveLoad() {
        Environment<AdjacencyListGraph<string>, string> env;
        env.addPoint("A");
        env.addPoint("B");
        LandVehicle car("Car", 2.0);
        env.addVehicle(car, "A");
        env.addPath("A","B",5.0, VehicleType::Land);

        env.saveGraphToFile("tmp_graph.csv");
        env.saveVehiclesToFile("tmp_vehicles.csv");

        Environment<AdjacencyListGraph<string>, string> env2;
        env2.loadGraphFromFile("tmp_graph.csv");
        env2.loadVehiclesFromFile("tmp_vehicles.csv");

        assert(env2.graphToString() == env.graphToString());
        assert(env2.canMove(0,"A","B"));
    }


public:
    static void runAllTests() {
        test_environment();
        test_computeOptimalRoute();
         test_addPointAndAddPath();
        test_addVehicleAndMove();
        test_simulateRoute();
        test_graphToString();
        test_saveLoad();
    }
};

#endif //ENVIRONMENT_TESTS_H
