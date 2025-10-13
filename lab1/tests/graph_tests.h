#ifndef GRAPHTESTS_H
#define GRAPHTESTS_H

#include <cassert>
#include "adjlist.h"
#include "matrix.h"
#include <string>
#include "vehicle.h"
#include "rand.h"

class GraphTests {
private:
    static void test_addVertexAndEdge_AdjList() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addEdge("A","B",3.0);
        assert(g.distance("A","B") == 3.0 && "Distance A->B should be 3.0");
        assert(g.isConnected() && "Graph should be connected");
    }

    static void test_removeVertex_AdjList() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        g.addEdge("A","B",1.0);
        g.addEdge("B","C",1.0);

        g.removeVertex("B");
        assert(!g.isConnected() && "Graph should not be connected after removing vertex B");
        assert(g.distance("A","C") == -1 && "Distance A->C should be -1 after removing B");
    }

    static void test_removeEdge_AdjList() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addEdge("A","B",2.0);

        g.removeEdge("A","B");
        assert(g.distance("A","B") == -1 && "Distance A->B should be -1 after removing edge");
        assert(!g.isConnected() && "Graph should not be connected after removing edge");
    }

    static void test_isConnected_AdjList() {
        AdjacencyListGraph<std::string> g(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");

        assert(!g.isConnected() && "Graph should not be connected initially");

        g.addEdge("A","B");
        g.addEdge("B","C");
        assert(g.isConnected() && "Graph should be connected after adding edges");
    }

    static void test_addVertexAndEdge_AdjMatrix() {
        AdjacencyMatrixGraph<std::string> g(false);
        g.addVertex("X");
        g.addVertex("Y");
        g.addEdge("X","Y",5.0);
        assert(g.distance("X","Y") == 5.0 && "Distance X->Y should be 5.0");
        assert(g.isConnected() && "Graph should be connected");
    }

    static void test_removeVertex_AdjMatrix() {
        AdjacencyMatrixGraph<std::string> g(false);
        g.addVertex("X");
        g.addVertex("Y");
        g.addVertex("Z");
        g.addEdge("X","Y",1.0);
        g.addEdge("Y","Z",1.0);

        g.removeVertex("Y");
        assert(!g.isConnected() && "Graph should not be connected after removing vertex Y");
        assert(g.distance("X","Z") == -1 && "Distance X->Z should be -1 after removing Y");
    }

    static void test_removeEdge_AdjMatrix() {
        AdjacencyMatrixGraph<std::string> g(false);
        g.addVertex("X");
        g.addVertex("Y");
        g.addEdge("X","Y",2.0);

        g.removeEdge("X","Y");
        assert(g.distance("X","Y") == -1 && "Distance X->Y should be -1 after removing edge");
        assert(!g.isConnected() && "Graph should not be connected after removing edge");
    }

    static void test_isConnected_AdjMatrix() {
        AdjacencyMatrixGraph<std::string> g(false);
        g.addVertex("X");
        g.addVertex("Y");
        g.addVertex("Z");

        assert(!g.isConnected() && "Graph should not be connected initially");

        g.addEdge("X","Y");
        g.addEdge("Y","Z");
        assert(g.isConnected() && "Graph should be connected after adding edges");
    }

    static void test_randomGraph() {
    // Генеруємо граф з випадковими int вершинами
    AdjacencyListGraph<int> g(false);
    g.generateRandom(5, 3, 10);  // 5 вершин, до 3 ребер на вершину, max вага 10
    std::cout << "Random Graph<int> adjacency list:\n";
    std::cout << g.toString() << "\n";

    // Генеруємо граф з випадковими BaseVehicle
    AdjacencyListGraph<BaseVehicle> vg(false);
    BaseVehicle prototype;
    vg.generateRandom(3, 2, 10, prototype);
    std::cout << "Random Graph<BaseVehicle> adjacency list:\n";
    for (int i = 0; i < vg.graphSize(); i++) {
        const BaseVehicle& v = vg.getVertex(i);
        std::cout << v.toString() << " -> ";
        auto neighbors = vg.getAdj(i);
        for (auto& [neighborIndex, weight] : neighbors) {
            const BaseVehicle& n = vg.getVertex(neighborIndex);
            std::cout << n.getName() << "(" << weight << ") ";
        }
        std::cout << "\n";
    }
}

    static void test_generateRandomEdge() {
        // int
        int maxInt = 10;
        int rInt = generateRandomEdge(maxInt);
        assert(rInt >= 0 && rInt <= maxInt);

        // double
        double maxDouble = 5.0;
        double rDouble = generateRandomEdge(maxDouble);
        assert(rDouble >= 0 && rDouble <= maxDouble); // якщо переписати шаблон під double

        // string
        std::string rStr = generateRandomEdge(std::string{});
        assert(!rStr.empty());
        assert(rStr.length() >= 3 && rStr.length() <= 7);

        // vector<int>
        std::vector<int> rVec = generateRandomEdge(std::vector<int>{});
        assert(!rVec.empty());
        for(int x : rVec) assert(x >= 0 && x < 100);

        // BaseVehicle
        BaseVehicle bv = generateRandomEdge(BaseVehicle{});
        assert(!bv.getName().empty());
        assert(bv.getWeight() > 0);
    }

    static void test_primMST_AdjList() {
        AdjacencyListGraph<int> g(false);
        g.addVertex(1); g.addVertex(2); g.addVertex(3);
        g.addEdge(1,2,4); g.addEdge(2,3,3); g.addEdge(1,3,5);

        auto mst = g.primMST();
        assert(mst.size() == 2); // numVertices - 1

        double totalWeight = 0;
        for (auto& [u,v] : mst) {
            if ((u==1 && v==2) || (u==2 && v==1)) totalWeight += 4;
            if ((u==2 && v==3) || (u==3 && v==2)) totalWeight += 3;
        }
        assert(totalWeight <= 7);
    }

    static void test_primMST_AdjMatrix() {
        AdjacencyMatrixGraph<int> g(false);
        g.addVertex(1); g.addVertex(2); g.addVertex(3);
        g.addEdge(1,2,4); g.addEdge(2,3,3); g.addEdge(1,3,5);

        auto mst = g.primMST();
        assert(mst.size() == 2);

        double totalWeight = 0;
        for (auto& [u,v] : mst) {
            if ((u==1 && v==2) || (u==2 && v==1)) totalWeight += 4;
            if ((u==2 && v==3) || (u==3 && v==2)) totalWeight += 3;
        }
        assert(totalWeight <= 7);
    }

    static void test_directedGraph_AdjList() {
        AdjacencyListGraph<int> g(true);
        g.addVertex(1); g.addVertex(2);
        g.addEdge(1,2,10);

        assert(g.distance(1,2) == 10);
        assert(g.distance(2,1) == -1);
    }

    static void test_directedGraph_AdjMatrix() {
        AdjacencyMatrixGraph<int> g(true);
        g.addVertex(1); g.addVertex(2);
        g.addEdge(1,2,10);

        assert(g.distance(1,2) == 10);
        assert(g.distance(2,1) == -1);
    }

    static void test_emptyGraph() {
        AdjacencyListGraph<int> g1(false);
        assert(g1.isConnected());
        assert(g1.distance(1,2) == -1);

        AdjacencyMatrixGraph<int> g2(false);
        assert(g2.isConnected());
        assert(g2.distance(1,2) == -1);
    }

    static void test_singleVertexGraph() {
        AdjacencyListGraph<int> g1(false);
        g1.addVertex(42);
        assert(g1.isConnected());
        assert(g1.distance(42,42) == 0);

        AdjacencyMatrixGraph<int> g2(false);
        g2.addVertex(42);
        assert(g2.isConnected());
        assert(g2.distance(42,42) == 0);
    }

    static void test_toString_AdjList() {
        AdjacencyListGraph<int> g(false);
        g.addVertex(1); g.addVertex(2);
        g.addEdge(1,2,7);

        std::string s = g.toString();
        assert(s.find("1 -> 2(7)") != std::string::npos);
    }

    static void test_toString_AdjMatrix() {
        AdjacencyMatrixGraph<int> g(false);
        g.addVertex(1); g.addVertex(2);
        g.addEdge(1,2,7);

        std::string s = g.toString();
        assert(s.find("1") != std::string::npos);
        assert(s.find("2") != std::string::npos);

        assert(s.find("7") != std::string::npos);
        assert(g.distance(1, 2) == 7.0);
    }


public:
    static void runAllTests() {
        test_addVertexAndEdge_AdjList();
        test_removeVertex_AdjList();
        test_removeEdge_AdjList();
        test_isConnected_AdjList();

        test_addVertexAndEdge_AdjMatrix();
        test_removeVertex_AdjMatrix();
        test_removeEdge_AdjMatrix();
        test_isConnected_AdjMatrix();
        test_generateRandomEdge();
        test_primMST_AdjList();
        test_primMST_AdjMatrix();
        test_directedGraph_AdjList();
        test_directedGraph_AdjMatrix();
        test_emptyGraph();
        test_singleVertexGraph();
        test_toString_AdjList();
        test_toString_AdjMatrix();
    }


};

#endif // GRAPHTESTS_H
