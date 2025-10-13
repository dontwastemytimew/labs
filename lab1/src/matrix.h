#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include "graph.h"
using namespace std;

// Клас для графа, представленого матрицею суміжності

template <typename T>
class AdjacencyMatrixGraph : public Graph<T> {
private:
    vector<T> vertices;                  // вершини
    vector<vector<double>> adjMatrix;    // матриця суміжності (0 або вага ребра)
    bool directed;                       // орієнтованість графа

    int getIndex(const T& v) const {
        for (int i = 0; i < vertices.size(); i++)
            if (vertices[i] == v) return i;
        return -1;
    }

public:
    explicit AdjacencyMatrixGraph(bool isDirected = false) : directed(isDirected) {}

    bool isDirected() const { return directed; }

    void addVertex(const T& v) override {
        if (getIndex(v) != -1) return;

        vertices.push_back(v);
        int n = vertices.size();
        adjMatrix.resize(n);
        for (auto &row : adjMatrix) row.resize(n, 0.0);
    }

    void removeVertex(const T& v) override {
        int idx = getIndex(v);
        if (idx == -1) return;

        vertices.erase(vertices.begin() + idx);
        adjMatrix.erase(adjMatrix.begin() + idx);
        for (auto& row : adjMatrix)
            row.erase(row.begin() + idx);
    }

    void addEdge(const T& v1, const T& v2, double weight = 1.0) override {
        int i1 = getIndex(v1), i2 = getIndex(v2);
        if (i1 == -1 || i2 == -1) return;

        adjMatrix[i1][i2] = weight;
        if (!directed)
            adjMatrix[i2][i1] = weight;
    }

    void removeEdge(const T& v1, const T& v2) override {
        int i1 = getIndex(v1), i2 = getIndex(v2);
        if (i1 == -1 || i2 == -1) return;

        adjMatrix[i1][i2] = 0.0;
        if (!directed)
            adjMatrix[i2][i1] = 0.0;
    }

    bool isConnected() const override {
        if (vertices.empty()) return true;

        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < vertices.size(); v++)
                if (adjMatrix[u][v] != 0 && !visited[v]) {
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

            for (int v = 0; v < vertices.size(); v++) {
                if (adjMatrix[u][v] != 0 && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    pq.push({dist[v], v});
                }
            }
        }

        return dist[i2] == INF ? -1 : dist[i2];
    }

    // Генерує граф випадковими вершинами та ребрами
    void generateRandom(int numVertices, int maxEdgesPerVertex, double maxWeight = 10.0, const T& prototypeVertex = T()) {
        if (numVertices <= 0) return;
        vertices.clear();
        adjMatrix.clear();

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
        cout << "Graph (Adjacency Matrix) " << (directed ? "[Directed]" : "[Undirected]") << ":\n    ";
        for (auto& v : vertices) cout << v << " ";
        cout << "\n";

        for (int i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << " ";
            for (int j = 0; j < vertices.size(); j++)
                cout << adjMatrix[i][j] << " ";
            cout << "\n";
        }
    }

    // Повернення текстового представлення
    string toString() const {
        stringstream ss;
        ss << "Graph (Adjacency Matrix) " << (directed ? "[Directed]" : "[Undirected]") << ":\n    ";
        for (auto& v : vertices) ss << v << " ";
        ss << "\n";

        for (int i = 0; i < vertices.size(); i++) {
            ss << vertices[i] << " ";
            for (int j = 0; j < vertices.size(); j++)
                ss << adjMatrix[i][j] << " ";
            ss << "\n";
        }
        return ss.str();
    }

    // Повертає список суміжності вершини (індекс + вага)
    vector<pair<int,double>> getAdj(int idx) const {
        vector<pair<int,double>> edges;
        if (idx < 0 || idx >= vertices.size()) return edges;
        for (int i = 0; i < vertices.size(); i++) {
            if (adjMatrix[idx][i] != 0)
                edges.push_back({i, adjMatrix[idx][i]});
        }
        return edges;
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
            for (int v = 0; v < vertices.size(); v++) {
                if (!inMST[v] && adjMatrix[u][v] != 0 && adjMatrix[u][v] < key[v]) {
                    key[v] = adjMatrix[u][v];
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



#endif //MATRIX_H
