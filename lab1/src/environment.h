#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include "vehicle.h"
#include <map>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

// Environment – поєднує граф і транспорт.
// canMove() перевіряє, чи транспорт може рухатися по дорозі (за типом).
// moveVehicle() – фактичний рух транспорту; враховується відстань через граф.
// simulateRoute() – послідовне переміщення транспорту по маршруту, можна додати візуалізацію через затримку.

class Road {
private:
    string from;
    string to;
    double cost;   // довжина або ефективна вага
    int type;   // 0 - Land, 1 - Water, 2 - Air

public:
    Road(const string& f, const string& t, int c, int tp)
        : from(f), to(t), cost(c), type(tp) {}

    string getFrom() const { return from; }
    string getTo() const { return to; }
    int getCost() const { return cost; }
    int getType() const { return type; }
};

template <typename GraphType, typename VertexT>
class Environment {
private:
    GraphType graph;                  // будь-який граф
    vector<BaseVehicle> vehicles;     // список транспортних засобів
    vector<VertexT> vehiclePositions; // поточні позиції транспортних засобів
    map<pair<VertexT, VertexT>, VehicleType> roadTypes; // Для перевірки типу дороги між вершинами
    vector<Road> roads;  // список доріг, реально зберігаємо об’єкти Road

public:
    Environment() = default;

    Environment(const GraphType& g) : graph(g) {}

    // Додати вершину (пункт)
    void addPoint(const VertexT& point) {
        graph.addVertex(point);
    }

    // Додати ребро (дорогу) з вагою/відстанню і типом дороги
    void addPath(const VertexT& from, const VertexT& to, double distance, VehicleType type) {
        graph.addEdge(from, to, distance);

        roadTypes[{from, to}] = type;
        if (!graph.isDirected()) roadTypes[{to, from}] = type;

        roads.emplace_back(from, to, static_cast<int>(distance), static_cast<int>(type));
    }

    void addVehicle(const BaseVehicle& vehicle, const VertexT& startPosition) {
        vehicles.push_back(vehicle);
        vehiclePositions.push_back(startPosition);
    }

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

    // Перевірка чи транспорт може рухатися між двома вершинами (Для AirVehicle дозволяємо рух по будь-яких дорогах)
    bool canMove(int vehicleIndex, const VertexT& from, const VertexT& to) const {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return false;
        auto it = roadTypes.find({from, to});
        if (it == roadTypes.end()) return false;

        VehicleType roadType = it->second;
        return roadType == vehicles[vehicleIndex].getType() || vehicles[vehicleIndex].getType() == VehicleType::Air;
    }

    // Рух транспортного засобу до вершини
    void moveVehicle(int vehicleIndex, const VertexT& destination, bool simulate = false, int delayMs = 500) {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) {
            cout << "Invalid vehicle index!\n";
            return;
        }

        if (vehicleIndex >= vehiclePositions.size()) {
            vehiclePositions.resize(vehicles.size());
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

        if (simulate) this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    // Моделювання маршруту (послідовність вершин)
    void simulateRoute(int vehicleIndex, const vector<VertexT>& route, int delayMs = 500) {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return;

        for (size_t i = 1; i < route.size(); i++) {
            moveVehicle(vehicleIndex, route[i], true, delayMs);
        }
    }

    // Обчислити найкоротший шлях для транспортного засобу
    vector<VertexT> computeOptimalRoute(int vehicleIndex, const VertexT& from, const VertexT& to) const {
        vector<VertexT> route;

        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return route;

        int indexFrom = graph.getVertexIndex(from);
        int indexTo = graph.getVertexIndex(to);

        if (indexFrom == -1 || indexTo == -1) return route;

        vector<double> dist(graph.graphSize(), 1e9);
        vector<int> prev(graph.graphSize(), -1);

        dist[indexFrom] = 0;
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        pq.push({0, indexFrom});

        while (!pq.empty()) {
            int u = pq.top().second; pq.pop();
            for (auto& [v, w] : graph.getAdj(u)) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        int cur = indexTo;
        if (prev[cur] == -1 && indexFrom != indexTo) return route;

        while (cur != -1) {
            route.insert(route.begin(), graph.getVertex(cur));
            cur = prev[cur];
        }

        return route;
    }

    // Повертає текстове представлення внутрішнього графа
    string graphToString() const {
        return graph.toString();
    }

    // Метод, який повертає всі дороги у вигляді тексту
    string roadsToString() const {
        stringstream ss;
        ss << "Roads in the environment:\n";
        for (const auto& road : roads) {
            string typeStr = (road.getType() == 0 ? "Land" :
                              road.getType() == 1 ? "Water" : "Air");
            ss << road.getFrom() << " -> " << road.getTo()
               << " (cost: " << road.getCost() << ", type: " << typeStr << ")\n";
        }
        return ss.str();
    }

    // Повертає поточну позицію транспортного засобу за його індексом
    VertexT getVehiclePosition(int vehicleIndex) const {
        if (vehicleIndex < 0 || vehicleIndex >= vehiclePositions.size()) {
            throw out_of_range("Invalid vehicle index");
        }
        return vehiclePositions[vehicleIndex];
    }

    void loadGraphFromFile(const std::string& filename) {
        ifstream file(filename);
        if (!file) return;
        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            stringstream ss(line);
            string from, to, typeStr, distStr;
            double distance;
            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, distStr, ',');
            distance = std::stod(distStr);
            getline(ss, typeStr, ',');

            VehicleType type = (typeStr == "Land" ? VehicleType::Land :
                                typeStr == "Water" ? VehicleType::Water :
                                VehicleType::Air);
            addPoint(from);
            addPoint(to);
            addPath(from, to, distance, type);
        }
    }


    void loadVehiclesFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            stringstream ss(line);
            string name, typeStr, speedStr, position;
            double speed;

            getline(ss, name, ',');
            getline(ss, typeStr, ',');
            getline(ss, speedStr, ',');
            speed = std::stod(speedStr);
            getline(ss, position, ',');

            BaseVehicle* v = nullptr;

            if (name == "Car") v = new Car(name, speed);
            else if (name == "Truck") v = new Truck(name, speed);
            else if (name == "SailBoat") v = new SailBoat(name, speed);
            else if (name == "MotorBoat") v = new MotorBoat(name, speed);
            else if (name == "Plane") v = new Plane(name, speed);
            else if (name == "Helicopter") v = new Helicopter(name, speed);
            else {
                // fallback — просто базовий транспорт
                VehicleType t = (typeStr == "Land" ? VehicleType::Land :
                                typeStr == "Water" ? VehicleType::Water : VehicleType::Air);
                v = new BaseVehicle(name, t, speed);
            }

            addVehicle(*v, position);
            delete v;
        }
    }


    void saveGraphToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) return;

        for (const auto& road : roads) {
            string typeStr = (road.getType() == 0 ? "Land" :
                                  road.getType() == 1 ? "Water" : "Air");
            file << road.getFrom() << ","
                 << road.getTo() << ","
                 << road.getCost() << ","
                 << typeStr << "\n";
        }
    }

    void saveVehiclesToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) return;

        for (size_t i = 0; i < vehicles.size(); ++i) {
            const BaseVehicle& v = vehicles[i];
            string className;

            // Визначаємо реальний клас об'єкта
            if (typeid(v) == typeid(Car)) className = "Car";
            else if (typeid(v) == typeid(Truck)) className = "Truck";
            else if (typeid(v) == typeid(SailBoat)) className = "SailBoat";
            else if (typeid(v) == typeid(MotorBoat)) className = "MotorBoat";
            else if (typeid(v) == typeid(Plane)) className = "Plane";
            else if (typeid(v) == typeid(Helicopter)) className = "Helicopter";
            else className = v.getName();

            // Конвертація типу у текст
            string typeStr = (v.getType() == VehicleType::Land ? "Land" :
                                   v.getType() == VehicleType::Water ? "Water" : "Air");

            // Запис у файл CSV
            file << className << ","      // Назва класу
                 << typeStr << ","        // Вид транспорту
                 << v.getWeight() << ","  // Швидкість/вага
                 << vehiclePositions[i]   // Позиція в графі
                 << "\n";
        }
    }




};


#endif // ENVIRONMENT_H
