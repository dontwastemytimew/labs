#ifndef ADJLIST_H
#define ADJLIST_H

#include <iostream>
#include "Graph.h"
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

template <typename T>
class AdjacencyListGraph : public Graph<T> {
private:
    vector<T> vertices;                              // список вершин
    vector<vector<pair<int, int>>> adjList;          // список суміжності (номер вершини, вага ребра)

    int getVertexIndex(const T& v) const {
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == v) {
                return i;
            }
        }
        return -1;
    }

public:
    AdjacencyListGraph() = default;

    void addVertex(const T& v) {
        if (getVertexIndex(v) == -1) {
            vertices.push_back(v);
            adjList.push_back({});
        } else {
            cout << "Vertex already exists" << endl;
        }
    }

    void removeVertex(const T& v) {
        int idx = getVertexIndex(v);
        if (idx == -1) {
            cout << "Vertex not found\n";
            return;
        }

        // видаляємо саму вершину з vertices
        vertices.erase(vertices.begin() + idx);

        // видаляємо список суміжності для цієї вершини
        adjList.erase(adjList.begin() + idx);

        // проходимо по всіх інших списках суміжності
        for (int i = 0; i < adjList.size(); i++) {
            // видаляємо ребра, які ведуть до idx
            for (int j = 0; j < adjList[i].size(); ) {
                if (adjList[i][j].first == idx) {
                    adjList[i].erase(adjList[i].begin() + j);
                } else if (adjList[i][j].first > idx) {
                    adjList[i][j].first--;
                    j++;
                } else {
                    j++;
                }
            }
        }
    }

    void addEdge(const T& v1, const T& v2, int weight) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "Vertex not found!" << endl;
            return;
        }

        for (auto& edge : adjList[idx1])
            if (edge.first == idx2) return; // ребро вже є

        adjList[idx1].push_back({idx2, weight});

        // якщо неорієнтований
        // adjList[idx2].push_back({idx1, weight});
    }

    void removeEdge(const T& v1, const T& v2) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "Vertex not found!" << endl;
            return;
        }

        // видаляємо індекс кінцевої вершини зі списку суміжності
        for (int i = 0; i < adjList[idx1].size(); i++) {
            if (adjList[idx1][i].first == idx2) {
                adjList[idx1].erase(adjList[idx1].begin() + i);
                break;
            }
        }
        // для неорієнтованого
        // for (int i = 0; i < adjList[idx2].size(); i++) {
        //    if (adjList[idx2][i].first == idx1) {
        //        adjList[idx2].erase(adjList[idx2].begin() + i);
        //        break;
        //    }
        // }
    }

    bool isConnected() const override {
        if (vertices.empty()) return true;

        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (auto &edge : adjList[current]) {
                int neighbor = edge.first;
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        for (bool v : visited)
            if (!v) return false;

        return true;
    }

    int distance(const T& v1, const T& v2) const override {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "Vertex not found!" << endl;
            return -1;
        }

        const int INF = 1e9;
        vector<int> dist(vertices.size(), INF);
        vector<bool> visited(vertices.size(), false);

        dist[idx1] = 0;

        for (int i = 0; i < vertices.size(); i++) {
            int u = -1;

            for (int j = 0; j < vertices.size(); j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }

            if (u == -1 || dist[u] == INF) break;
            visited[u] = true;

            for (auto& edge : adjList[u]) {
                int neighbor = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[u] + weight;
                }
            }
        }

        if (dist[idx2] == INF) {
            cout << "No path between vertices!" << endl;
            return -1;
        }

        return dist[idx2];
    }


};


#endif //ADJLIST_H
