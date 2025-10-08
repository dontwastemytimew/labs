#ifndef GRAPH_H
#define GRAPH_H

// Базовий клас графа, його завдання — визначити інтерфейс, тобто перелік методів,
// які обов’язково повинні реалізувати підкласи (список суміжності, матриця суміжності).

template <typename vertexT, typename edgeT>
class Graph {
public:
    virtual ~Graph() = default;

    virtual void addVertex(const vertexT& v, const edgeT& edgeData = edgeT()) = 0;
    virtual void removeVertex(const vertexT& v) = 0;
    virtual void addEdge(const vertexT& v1, const vertexT& v2, const edgeT& weight) = 0;
    virtual void removeEdge(const vertexT& v1, const vertexT& v2) = 0;
    virtual bool isConnected() const = 0;
    virtual double distance(const vertexT& v1, const vertexT& v2) const = 0;
};

#endif //GRAPH_H
