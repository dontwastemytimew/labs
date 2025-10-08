#ifndef ADJLIST_H
#define ADJLIST_H

#include <iostream>
#include "graph.h"
#include "data.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include "rand.h"
using namespace std;

// Клас для графа, представленого списком суміжності

template <typename vertexT, typename edgeT = int>
class AdjacencyListGraph : public Graph<vertexT, edgeT> {
private:
    vector<Data<vertexT, edgeT>> vertices;                // вершини з даними
    vector<vector<Data<vertexT, edgeT>>> adjList;         // список суміжності
    bool directed;

    int getVertexIndex(const vertexT& v) const {
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i].vertexData == v)
                return i;
        }
        return -1;
    }

public:
    explicit AdjacencyListGraph(bool isDirected = false) : directed(isDirected) {}

    bool isDirected() const { return directed; }

    void addVertex(const vertexT& v, const edgeT& edgeData = edgeT()) override {
        if (getVertexIndex(v) != -1) {
            cout << "Vertex already exists\n";
            return;
        }
        int num = vertices.size();
        vertices.push_back(Data<vertexT, edgeT>(v, edgeData, num));
        adjList.push_back({});
    }

    void removeVertex(const vertexT& v) {
        int idx = getVertexIndex(v);
        if (idx == -1) { cout << "Vertex not found\n"; return; }

        vertices.erase(vertices.begin() + idx);
        adjList.erase(adjList.begin() + idx);

        for (int i = 0; i < adjList.size(); i++) {
            for (int j = 0; j < adjList[i].size(); ) {
                if (adjList[i][j].number == idx) {
                    adjList[i].erase(adjList[i].begin() + j);
                } else {
                    if (adjList[i][j].number > idx)
                        adjList[i][j].number--;
                    j++;
                }
            }
        }

        for (int i = 0; i < vertices.size(); i++)
            if (vertices[i].number > idx)
                vertices[i].number--;
    }

    void addEdge(const vertexT& v1, const vertexT& v2, const edgeT& edgeData) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);
        if (idx1 == -1 || idx2 == -1) { cout << "Vertex not found\n"; return; }

        adjList[idx1].push_back(Data<vertexT, edgeT>(vertexT{}, edgeData, idx2));
if (!directed)
    adjList[idx2].push_back(Data<vertexT, edgeT>(vertexT{}, edgeData, idx1));

    }

    void removeEdge(const vertexT& v1, const vertexT& v2) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);
        if (idx1 == -1 || idx2 == -1) { cout << "Vertex not found\n"; return; }

        adjList[idx1].erase(remove_if(adjList[idx1].begin(), adjList[idx1].end(),
                                      [idx2](const Data<vertexT, edgeT>& d){ return d.number == idx2; }),
                            adjList[idx1].end());
        if (!directed)
            adjList[idx2].erase(remove_if(adjList[idx2].begin(), adjList[idx2].end(),
                                          [idx1](const Data<vertexT, edgeT>& d){ return d.number == idx1; }),
                                adjList[idx2].end());
    }

    bool isConnected() const {
        if (vertices.empty()) return true;

        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& edge : adjList[u]) {
                int v = edge.number;
                if (!visited[v]) { visited[v] = true; q.push(v); }
            }
        }

        for (bool v : visited) if (!v) return false;
        return true;
    }


    double distance(const vertexT& v1, const vertexT& v2) const override {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);
        if (idx1 == -1 || idx2 == -1) { cout << "Vertex not found\n"; return -1; }

        const double INF = 1e9;
        vector<double> dist(vertices.size(), INF);
        vector<bool> visited(vertices.size(), false);
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

        dist[idx1] = 0;
        pq.push({0, idx1});

        while (!pq.empty()) {
            int u = pq.top().second; pq.pop();
            if (visited[u]) continue;
            visited[u] = true;

            for (auto& edge : adjList[u]) {
                int v = edge.number;
                double w = edge.getWeight();
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[idx2] == INF) { cout << "No path\n"; return -1; }
        return dist[idx2];
    }

    // Генерує граф випадковими вершинами та ребрами
    void generateRandom(int numVertices, int maxEdgesPerVertex, const edgeT& maxWeight = edgeT(), const vertexT& prototypeVertex = vertexT()) {
        vertices.clear();
        adjList.clear();

        // Додаємо вершини
        for (int i = 0; i < numVertices; i++)
            addVertex(prototypeVertex);

        // Додаємо випадкові ребра
        for (int i = 0; i < numVertices; i++) {
            int edges = rand() % (maxEdgesPerVertex + 1);
            for (int j = 0; j < edges; j++) {
                int to = rand() % numVertices;
                if (to != i) {
                    edgeT weight = generateRandomEdge(maxWeight);
                    addEdge(vertices[i].vertexData, vertices[to].vertexData, weight);
                }
            }
        }
    }



    void printGraph() const {
        cout << "Graph (Adjacency List):\n";
        for (int i = 0; i < vertices.size(); i++) {
            cout << vertices[i].vertexData << " -> ";
            for (auto& edge : adjList[i]) {
                cout << edgeToString(edge.edgeData) << " ";
            }
            cout << "\n";
        }
    }

    // Повернення текстового представлення
    string toString() const {
        stringstream ss;
        ss << "Graph (Adjacency List):\n";
        for (int i = 0; i < vertices.size(); i++) {
            ss << vertices[i].vertexData << " -> ";
            for (auto& edge : adjList[i]) {
                ss << vertices[edge.number].vertexData << "(" << edge.edgeData << ") ";
            }
            ss << "\n";
        }
        return ss.str();
    }


};


#endif //ADJLIST_H
