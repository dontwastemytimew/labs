#ifndef ADJLIST_H
#define ADJLIST_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include "graph.h"
using namespace std;

// Клас для графа, представленого списком суміжності

template <typename T>
class AdjacencyListGraph : public Graph<T> {
private:
    vector<T> vertices;                         // вершини
    vector<vector<pair<int,double>>> adjList;   // список суміжності: {індекс вершини, вага}
    bool directed;                              // орієнтованість графа

    int getIndex(const T& v) const {
        for (int i = 0; i < vertices.size(); i++)
            if (vertices[i] == v) return i;
        return -1;
    }

public:
    explicit AdjacencyListGraph(bool isDirected = false) : directed(isDirected) {}

    bool isDirected() const { return directed; }

    void addVertex(const T& v) override {
        if (getIndex(v) != -1) return;
        vertices.push_back(v);
        adjList.push_back({});
    }

    void removeVertex(const T& v) {
        int idx = getIndex(v);
        if (idx == -1) {
            cout << "Vertex not found\n";
            return;
        }

        vertices.erase(vertices.begin() + idx);
        adjList.erase(adjList.begin() + idx);

        for (auto& edges : adjList) {
            for (auto it = edges.begin(); it != edges.end();) {
                if (it->first == idx)
                    it = edges.erase(it);
                else {
                    if (it->first > idx) it->first--;
                    ++it;
                }
            }
        }
    }

    void addEdge(const T& v1, const T& v2, double weight = 1.0) override {
        int i1 = getIndex(v1), i2 = getIndex(v2);
        if (i1 == -1 || i2 == -1) return;

        adjList[i1].push_back({i2, weight});
        if (!directed)
            adjList[i2].push_back({i1, weight});
    }

    void removeEdge(const T& v1, const T& v2) override {
        int idx1 = getIndex(v1);
        int idx2 = getIndex(v2);
        if (idx1 == -1 || idx2 == -1) {
            cout << "Vertex not found\n";
            return;
        }

        // Видаляємо ребро idx1 -> idx2
        for (auto it = adjList[idx1].begin(); it != adjList[idx1].end(); ) {
            if (it->first == idx2)
                it = adjList[idx1].erase(it);
            else
                ++it;
        }

        // Якщо граф неорієнтований, видаляємо idx2 -> idx1
        if (!directed) {
            for (auto it = adjList[idx2].begin(); it != adjList[idx2].end(); ) {
                if (it->first == idx1)
                    it = adjList[idx2].erase(it);
                else
                    ++it;
            }
        }
    }

    bool isConnected() const override {
        if (vertices.empty()) return true;

        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& [v, w] : adjList[u])
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
        }

        return all_of(visited.begin(), visited.end(), [](bool b){ return b; });
    }

    double distance(const T& v1, const T& v2) const override {
        int i1 = getVertexIndex(v1);
        int i2 = getVertexIndex(v2);
        if (i1 == -1 || i2 == -1) return -1;

        const double INF = 1e9;
        vector<double> dist(vertices.size(), INF);
        vector<bool> visited(vertices.size(), false);
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

        dist[i1] = 0;
        pq.push({0, i1});

        while (!pq.empty()) {
            int u = pq.top().second; pq.pop();
            if (u < 0 || u >= vertices.size()) continue;
            if (visited[u]) continue;
            visited[u] = true;

            for (auto& [v, w] : adjList[u]) {
                if (v < 0 || v >= vertices.size()) continue;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist[i2] == INF ? -1 : dist[i2];
    }


    // Генерує граф випадковими вершинами та ребрами
    void generateRandom(int numVertices, int maxEdgesPerVertex, double maxWeight = 10.0, const T& prototypeVertex = T()) {
        if (numVertices <= 0) return; // перевірка
        vertices.clear();
        adjList.clear();

        for (int i = 0; i < numVertices; i++)
            addVertex(prototypeVertex);

        for (int i = 0; i < numVertices; i++) {
            int edges = rand() % (maxEdgesPerVertex + 1);
            for (int j = 0; j < edges; j++) {
                int to = rand() % numVertices;
                if (to != i) {
                    double weight = 1.0 + rand() % static_cast<int>(maxWeight);
                    addEdge(vertices[i], vertices[to], weight);
                }
            }
        }
    }

    void printGraph() const {
        cout << "Graph (Adjacency List) " << (directed ? "[Directed]" : "[Undirected]") << ":\n";
        for (int i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << " -> ";
            for (auto& [v, w] : adjList[i])
                cout << vertices[v] << "(" << w << ") ";
            cout << "\n";
        }
    }

    // Повернення текстового представлення
    string toString() const {
        stringstream ss;
        ss << "Graph (Adjacency List) " << (directed ? "[Directed]" : "[Undirected]") << ":\n";
        for (int i = 0; i < vertices.size(); i++) {
            ss << vertices[i] << " -> ";
            for (auto& [v, w] : adjList[i])
                ss << vertices[v] << "(" << w << ") ";
            ss << "\n";
        }
        return ss.str();
    }

    // Повертає список суміжності вершини (індекс + вага)
    const vector<pair<int,double>>& getAdj(int idx) const {
        static vector<pair<int,double>> empty;
        if (idx < 0 || idx >= adjList.size()) return empty;
        return adjList[idx];
    }

    // Повертає кількість вершин у графі
    int graphSize() const { return vertices.size(); }

    int getVertexIndex(const T& v) const { return getIndex(v); }

    T getVertex(int idx) const {
        if (idx < 0 || idx >= vertices.size()) throw out_of_range("getVertex: index out of range");
        return vertices[idx];
    }

    // Алгоритм Пріма для побудови мінімального кістякового дерева
    vector<pair<T, T>> primMST() const {
        if (vertices.empty()) return {};

        const double INF = 1e9;
        vector<double> key(vertices.size(), INF);
        vector<int> parent(vertices.size(), -1);
        vector<bool> inMST(vertices.size(), false);

        key[0] = 0;

        for (int count = 0; count < vertices.size(); count++) {
            // Вибираємо вершину з мінімальною key, яка ще не в MST
            double minKey = INF;
            int u = -1;
            for (int i = 0; i < vertices.size(); i++) {
                if (!inMST[i] && key[i] < minKey) {
                    minKey = key[i];
                    u = i;
                }
            }

            if (u == -1) break;
            inMST[u] = true;

            // Оновлюємо ключі суміжних вершин
            for (auto& [v, w] : adjList[u]) {
                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                }
            }
        }

        vector<pair<T, T>> mstEdges;
        for (int i = 1; i < vertices.size(); i++) {
            if (parent[i] != -1)
                mstEdges.push_back({vertices[parent[i]], vertices[i]});
        }
        return mstEdges;
    }


};


#endif //ADJLIST_H
