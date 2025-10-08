#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "vehicle.h"
#include "graph.h"
#include <map>

using namespace std;

// Environment – поєднує граф і транспорт.
// canMove() перевіряє, чи транспорт може рухатися по дорозі (за типом).
// moveVehicle() – фактичний рух транспорту; враховується відстань через граф.
// simulateRoute() – послідовне переміщення транспорту по маршруту, можна додати візуалізацію через затримку.
// Використання шаблону GraphType → можна підставити будь-який граф зі списком або матрицею суміжності.

template <typename GraphType, typename VertexT>
class Environment {
private:
    GraphType graph;                  // будь-який граф
    vector<BaseVehicle> vehicles;     // список транспортних засобів
    vector<VertexT> vehiclePositions; // поточні позиції транспортних засобів
    map<pair<VertexT, VertexT>, VehicleType> roadTypes; // Для перевірки типу дороги між вершинами

public:
    Environment() = default;

    // Додати вершину (пункт)
    void addPoint(const VertexT& point) {
        graph.addVertex(point);
    }

    // Додати ребро (дорогу) з вагою/відстанню і типом дороги
    void addPath(const VertexT& from, const VertexT& to, double distance, VehicleType type) {
        // Ми зберігаємо тільки вагу, тип дороги передаємо для перевірки в canMove
        graph.addEdge(from, to, distance);
        // Можна додати додаткову структуру map<pair<VertexT, VertexT>, VehicleType> для типу дороги
        roadTypes[{from, to}] = type;
        if (!graph.isDirected()) roadTypes[{to, from}] = type;
    }

    // Додати транспортний засіб на граф
    void addVehicle(const BaseVehicle& vehicle, const VertexT& startPosition) {
        vehicles.push_back(vehicle);
        vehiclePositions.push_back(startPosition);
    }

    // Показати всі транспортні засоби та їх позиції
    void showVehicles() const {
        for (size_t i = 0; i < vehicles.size(); i++) {
            const auto& v = vehicles[i];
            cout << "Vehicle " << v.getName()
                 << " at " << vehiclePositions[i]
                 << " (Type: "
                 << (v.getType() == VehicleType::Land ? "Land" :
                     v.getType() == VehicleType::Water ? "Water" : "Air")
                 << ")\n";
        }
    }

    // Перевірка чи транспорт може рухатися між двома вершинами
    bool canMove(int vehicleIndex, const VertexT& from, const VertexT& to) const {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return false;
        auto it = roadTypes.find({from, to});
        if (it == roadTypes.end()) return false;

        VehicleType roadType = it->second;
        return roadType == vehicles[vehicleIndex].getType() || vehicles[vehicleIndex].getType() == VehicleType::Air;
        // Для AirVehicle дозволяємо рух по будь-яких дорогах
    }

    // Рух транспортного засобу до вершини
    void moveVehicle(int vehicleIndex, const VertexT& destination, bool simulate = false, int delayMs = 500) {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) {
            cout << "Invalid vehicle index!\n";
            return;
        }

        const VertexT& current = vehiclePositions[vehicleIndex];
        if (!canMove(vehicleIndex, current, destination)) {
            cout << vehicles[vehicleIndex].getName()
                 << " cannot move from " << current << " to " << destination << " (wrong road type)\n";
            return;
        }

        double dist = graph.distance(current, destination);
        if (dist < 0) {
            cout << vehicles[vehicleIndex].getName() << " cannot reach " << destination << "\n";
            return;
        }

        cout << vehicles[vehicleIndex].getName()
             << " moves from " << current << " to " << destination
             << ", effective cost: " << dist << "\n";

        vehiclePositions[vehicleIndex] = destination;

        if (simulate) std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    // Моделювання маршруту (послідовність вершин)
    void simulateRoute(int vehicleIndex, const vector<VertexT>& route, int delayMs = 500) {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return;

        for (size_t i = 1; i < route.size(); i++) {
            moveVehicle(vehicleIndex, route[i], true, delayMs);
        }
    }


};

#endif // ENVIRONMENT_H
