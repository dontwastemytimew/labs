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


/**
 * @file environment.h
 * @brief Містить клас Road та шаблонний клас Environment для керування графом та транспортними засобами.
 */

/**
 * @brief Клас, який представляє дорогу між двома вершинами.
 */
class Road {
private:
    string from; ///< Початкова вершина
    string to;   ///< Кінцева вершина
    double cost; ///< Вартість/довжина дороги
    int type;    ///< Тип дороги: 0 - Land, 1 - Water, 2 - Air

public:
    /**
     * @brief Конструктор дороги
     * @param f Початкова вершина
     * @param t Кінцева вершина
     * @param c Вартість/довжина
     * @param tp Тип дороги
     */
    Road(const string& f, const string& t, int c, int tp)
        : from(f), to(t), cost(c), type(tp) {}

    string getFrom() const { return from; }
    string getTo() const { return to; }
    int getCost() const { return cost; }
    int getType() const { return type; }
};


/**
 * @brief Клас середовища, яке об’єднує граф і транспортні засоби.
 * @tparam GraphType Тип графа (Adjacency List/Matrix)
 * @tparam VertexT Тип вершин графа (наприклад, std::string)
 */
template <typename GraphType, typename VertexT>
class Environment {
private:
    GraphType graph;                                     ///< Граф середовища
    vector<BaseVehicle> vehicles;                        ///< Список транспортних засобів
    vector<VertexT> vehiclePositions;                    ///< Поточні позиції транспортних засобів
    map<pair<VertexT, VertexT>, VehicleType> roadTypes;  ///< Типи доріг між вершинами
    vector<Road> roads;                                  ///< Список об’єктів Road

public:
    Environment() = default;

    Environment(const GraphType& g) : graph(g) {}

    /** @brief Додати вершину (пункт) у граф */
    void addPoint(const VertexT& point) {
        graph.addVertex(point);
    }

    /**
    * @brief Додати ребро (дорогу) між вершинами
    * @param from Початкова вершина
    * @param to Кінцева вершина
    * @param distance Відстань або вага
    * @param type Тип транспортного засобу, який може рухатися
    */
    void addPath(const VertexT& from, const VertexT& to, double distance, VehicleType type) {
        graph.addEdge(from, to, distance);

        roadTypes[{from, to}] = type;
        if (!graph.isDirected()) roadTypes[{to, from}] = type;

        roads.emplace_back(from, to, static_cast<int>(distance), static_cast<int>(type));
    }

    /**
     * @brief Додати транспортний засіб до середовища
     * @param vehicle Транспортний засіб
     * @param startPosition Початкова позиція у графі
     */
    void addVehicle(const BaseVehicle& vehicle, const VertexT& startPosition) {
        vehicles.push_back(vehicle);
        vehiclePositions.push_back(startPosition);
    }

    /** @brief Вивести інформацію про всі транспортні засоби у середовищі */
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

    /**
      * @brief Перевіряє, чи транспортний засіб може рухатися між двома вершинами
      * @param vehicleIndex Індекс транспортного засобу
      * @param from Початкова вершина
      * @param to Кінцева вершина
      * @return true, якщо можна рухатися
      */
    bool canMove(int vehicleIndex, const VertexT& from, const VertexT& to) const {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return false;
        auto it = roadTypes.find({from, to});
        if (it == roadTypes.end()) return false;

        VehicleType roadType = it->second;
        return roadType == vehicles[vehicleIndex].getType() || vehicles[vehicleIndex].getType() == VehicleType::Air;
    }

    /**
    * @brief Рух транспортного засобу до вершини
    * @param vehicleIndex Індекс транспортного засобу
    * @param destination Кінцева вершина
    * @param simulate Якщо true, додає затримку для імітації руху
    * @param delayMs Затримка у мілісекундах
    */
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

    /**
    * @brief Імітація маршруту для транспортного засобу
    * @param vehicleIndex Індекс транспортного засобу
    * @param route Послідовність вершин маршруту
    * @param delayMs Затримка між кроками
    */
    void simulateRoute(int vehicleIndex, const vector<VertexT>& route, int delayMs = 500) {
        if (vehicleIndex < 0 || vehicleIndex >= vehicles.size()) return;

        for (size_t i = 1; i < route.size(); i++) {
            moveVehicle(vehicleIndex, route[i], true, delayMs);
        }
    }

    /** @brief Обчислити оптимальний маршрут між двома вершинами для транспортного засобу */
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

    /** @brief Повертає текстове представлення графа */
    string graphToString() const {
        return graph.toString();
    }

    /** @brief Повертає всі дороги у вигляді тексту */
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

    /** @brief Повертає поточну позицію транспортного засобу */
    VertexT getVehiclePosition(int vehicleIndex) const {
        if (vehicleIndex < 0 || vehicleIndex >= vehiclePositions.size()) {
            throw out_of_range("Invalid vehicle index");
        }
        return vehiclePositions[vehicleIndex];
    }

    /** @brief Зчитування графа з CSV файлу */
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


    /** @brief Зчитування транспортних засобів з CSV файлу */
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

    /** @brief Збереження графа у CSV файл */
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

    /** @brief Збереження транспортних засобів у CSV файл */
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
