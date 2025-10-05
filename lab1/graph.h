#ifndef GRAPH_H
#define GRAPH_H

template <typename T>
class Graph {
public:
    virtual ~Graph() = default;

    virtual void addVertex(const T& v) = 0;
    virtual void removeVertex(const T& v) = 0;

    virtual void addEdge(const T& v1, const T& v2, int weight = 1) = 0;
    virtual void removeEdge(const T& v1, const T& v2) = 0;

    virtual bool isConnected() const = 0;
    virtual int distance(const T& v1, const T& v2) const = 0;
};

#endif //GRAPH_H
