#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "graph.h"
#include "data.h"
#include <vector>
#include <queue>
#include "rand.h"
using namespace std;

// Клас для графа, представленого матрицею суміжності


template <typename vertexT, typename edgeT = int>
class AdjacencyMatrixGraph : public Graph<vertexT, edgeT> {
private:
    vector<Data<vertexT, edgeT>> vertices;                   // вершини з даними
    vector<vector<Data<vertexT, edgeT>>> adjMatrix;          // матриця суміжності
    bool directed;

    int getVertexIndex(const vertexT& v) const {
        for (int i = 0; i < vertices.size(); i++)
            if (vertices[i].vertexData == v) return i;
        return -1;
    }

public:
    explicit AdjacencyMatrixGraph(bool isDirected = false) : directed(isDirected) {}

    void addVertex(const vertexT& v, const edgeT& edgeData = edgeT()) override {
        if (getVertexIndex(v) != -1) { cout << "Vertex already exists\n"; return; }
        int num = vertices.size();
        vertices.push_back(Data<vertexT, edgeT>(v, edgeData, num));

        int n = vertices.size();
        adjMatrix.resize(n);
        for (auto &row : adjMatrix) row.resize(n);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                adjMatrix[i][j] = Data<vertexT, edgeT>();
    }

    void removeVertex(const vertexT& v) {
        int idx = getVertexIndex(v);
        if (idx == -1) { cout << "Vertex not found\n"; return; }

        vertices.erase(vertices.begin() + idx);
        adjMatrix.erase(adjMatrix.begin() + idx); // видаляємо рядок
        for (auto &row : adjMatrix)
            row.erase(row.begin() + idx);        // видаляємо стовпець

        for (int i = 0; i < vertices.size(); i++)
            if (vertices[i].number > idx)
                vertices[i].number--;
    }


    void addEdge(const vertexT& v1, const vertexT& v2, const edgeT& edgeData) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);
        if (idx1 == -1 || idx2 == -1) { cout << "Vertex not found\n"; return; }

        adjMatrix[idx1][idx2] = Data<vertexT, edgeT>(edgeData, idx2);
        if (!directed)
            adjMatrix[idx2][idx1] = Data<vertexT, edgeT>(edgeData, idx1);
    }

    void removeEdge(const vertexT& v1, const vertexT& v2) {
        int idx1 = getVertexIndex(v1);
        int idx2 = getVertexIndex(v2);
        if (idx1 == -1 || idx2 == -1) { cout << "Vertex not found\n"; return; }

        adjMatrix[idx1][idx2] = Data<vertexT, edgeT>();
        if (!directed)
            adjMatrix[idx2][idx1] = Data<vertexT, edgeT>();
    }

    bool isConnected() const {
        if (vertices.empty()) return true;

        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < vertices.size(); v++) {
                if (adjMatrix[u][v].number != -1 && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
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

            for (int v = 0; v < vertices.size(); v++) {
                if (adjMatrix[u][v].number != -1) {
                    double w = adjMatrix[u][v].getWeight();
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                    }
                }
            }
        }

        if (dist[idx2] == INF) { cout << "No path\n"; return -1; }
        return dist[idx2];
    }

    void generateRandom(int numVertices, int maxEdgesPerVertex, const edgeT& maxWeight = edgeT()) {
        vertices.clear();
        adjMatrix.clear();

        // Додаємо вершини
        for (int i = 0; i < numVertices; i++)
            addVertex(vertexT{});

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
        cout << "Graph (Adjacency Matrix):\n    ";
        for (auto& v : vertices) cout << v.vertexData << " ";
        cout << endl;

        for (int i = 0; i < vertices.size(); i++) {
            cout << vertices[i].vertexData << " ";
            for (int j = 0; j < vertices.size(); j++) {
                if (adjMatrix[i][j].number != -1)
                    cout << adjMatrix[i][j].edgeData << " ";
                else
                    cout << "0 ";
            }
            cout << endl;
        }
    }

    string toString() const {
        stringstream ss;
        ss << "Graph (Adjacency Matrix):\n    ";
        for (auto& v : vertices) ss << v.vertexData << " ";
        ss << "\n";

        for (int i = 0; i < vertices.size(); i++) {
            ss << vertices[i].vertexData << " ";
            for (int j = 0; j < vertices.size(); j++) {
                if (adjMatrix[i][j].number != -1)
                    ss << adjMatrix[i][j].edgeData << " ";
                else
                    ss << "0 ";
            }
            ss << "\n";
        }
        return ss.str();
    }


};


#endif //MATRIX_H
