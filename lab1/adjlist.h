#ifndef ADJLIST_H
#define ADJLIST_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

template <typename T>
class AdjacencyListGraph : public Graph<T> {
private:
    vector<T> vertices;                              // список вершин
    vector<vector<pair<int, int>>> adjList;          // список суміжності (номер вершини, вага ребра)

    int getVertexIndex(const T& v) {
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
        int idx = -1;
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == v) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            cout << "Vertex not found" << endl;
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
    // потім

    void addEdge(const T& v1, const T& v2);
    void removeEdge(const T& v1, const T& v2);
    bool isConnected();
    int distance(const T& v1, const T& v2);

};


#endif //ADJLIST_H
