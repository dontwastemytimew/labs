#include <iostream>
#include <vector>
#include <string>
#include "adjlist.h"
#include "environment.h"
#include "vehicle.h"
#include "rand.h"

using namespace std;

// Допоміжна функція для друку графа з BaseVehicle
void printVehicleGraph(const AdjacencyListGraph<BaseVehicle>& g) {
    cout << "Graph<BaseVehicle> adjacency list:\n";
    for (int i = 0; i < g.graphSize(); i++) {
        const BaseVehicle& v = g.getVertex(i);
        cout << v.getName() << " -> ";

        auto neighbors = g.getAdj(i);
        for (auto& [neighborIndex, weight] : neighbors) {
            const BaseVehicle& neighbor = g.getVertex(neighborIndex);
            cout << neighbor.getName() << " (" << weight << ") ";
        }
        cout << "\n";
    }
}

int main() {
    // Граф зі string
    AdjacencyListGraph<string> graphStr(false);
    graphStr.addVertex("A");
    graphStr.addVertex("B");
    graphStr.addVertex("C");
    graphStr.addEdge("A","B",5.0);
    graphStr.addEdge("B","C",3.0);

    cout << "Graph<string> adjacency list:\n";
    cout << graphStr.toString() << endl;

    // Граф з int
    AdjacencyListGraph<int> graphInt(false);
    graphInt.addVertex(1);
    graphInt.addVertex(2);
    graphInt.addVertex(3);
    graphInt.addEdge(1,2,4.0);
    graphInt.addEdge(2,3,2.0);

    cout << "Graph<int> adjacency list:\n";
    cout << graphInt.toString() << endl;

    // Граф з double
    AdjacencyListGraph<double> graphDouble(false);
    graphDouble.addVertex(1.1);
    graphDouble.addVertex(2.2);
    graphDouble.addVertex(3.3);
    graphDouble.addEdge(1.1,2.2,1.5);
    graphDouble.addEdge(2.2,3.3,2.5);

    cout << "Graph<double> adjacency list:\n";
    cout << graphDouble.toString() << endl;

    // Граф з vector<string>
    AdjacencyListGraph<vector<string>> graphVec(false);
    vector<string> v1{"A","B"};
    vector<string> v2{"C","D"};
    vector<string> v3{"E","F"};

    graphVec.addVertex(v1);
    graphVec.addVertex(v2);
    graphVec.addVertex(v3);

    graphVec.addEdge(v1, v2, 3.0);
    graphVec.addEdge(v2, v3, 1.0);

    cout << "Graph<vector<string>> adjacency list:\n";

    for (const auto& vec : {v1, v2, v3}) {
        int idx = graphVec.getVertexIndex(vec);
        if (idx < 0) {
            cout << "Vertex not found!\n";
            continue;
        }

        cout << "[";
        for (const auto& s : vec) cout << s << " ";
        cout << "] -> ";

        auto neighbors = graphVec.getAdj(idx);
        for (const auto& pair : neighbors) {
            const auto& neighbor = pair.first;
            cout << "[";
            for (const auto& s : neighbor) cout << s << " ";
            cout << "] ";
        }
        cout << "\n";
    }

    // Граф з BaseVehicle
    AdjacencyListGraph<BaseVehicle> graphVehicle(false);

    LandVehicle car("Car", 2.0);
    WaterVehicle boat("Boat", 1.0);
    AirVehicle plane("Plane", 5.0);

    graphVehicle.addVertex(car);
    graphVehicle.addVertex(boat);
    graphVehicle.addVertex(plane);

    graphVehicle.addEdge(car, boat, car.getWeight() + boat.getWeight());
    graphVehicle.addEdge(boat, plane, boat.getWeight() + plane.getWeight());
    graphVehicle.addEdge(car, plane, car.getWeight() + plane.getWeight());

    printVehicleGraph(graphVehicle);

    // Environment з транспортом
    AdjacencyListGraph<string> graphEnv(false);
    Environment<AdjacencyListGraph<string>, string> env(graphEnv);

    env.addPoint("A");
    env.addPoint("B");
    env.addPoint("C");
    env.addPath("A","B",5.0, VehicleType::Land);
    env.addPath("B","C",3.0, VehicleType::Water);

    env.addVehicle(car,"A");
    env.addVehicle(boat,"B");
    env.addVehicle(plane,"A");

    cout << "\nVehicles in Environment:\n";
    env.showVehicles();

    cout << "\nGraph in Environment:\n";
    cout << env.graphToString();

    cout << "\nSimulate route for Car:\n";
    env.simulateRoute(0, {"A","B"}); // car moves

    cout << "\nSimulate route for Plane:\n";
    env.simulateRoute(2, {"A","B","C"}); // plane moves

    // Генерація випадкового графа з int
    AdjacencyListGraph<int> randomGraph(false);
    randomGraph.generateRandom(5, 3);
    cout << "\nRandom Graph<int> adjacency list:\n";
    cout << randomGraph.toString() << endl;

    // Генерація випадкового графа з BaseVehicle
    AdjacencyListGraph<BaseVehicle> randomVehicleGraph(false);

    randomVehicleGraph.generateRandom(3, 2);

    cout << "\nRandom Graph<BaseVehicle> adjacency list:\n";
    printVehicleGraph(randomVehicleGraph);

    return 0;
}