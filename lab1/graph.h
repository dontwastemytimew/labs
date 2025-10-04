#ifndef ADJLIST_H
#define ADJLIST_H

template <typename T>
class Graph {
public:
    virtual ~Graph() {}

    virtual void addVertex(const T& v) = 0;
    virtual void removeVertex(const T& v) = 0;

    virtual void addEdge(const T& v1, const T& v2) = 0;
    virtual void removeEdge(const T& v1, const T& v2) = 0;

    virtual bool isConnected() const = 0;
    virtual int distance(const T& v1, const T& v2) const = 0;
};


#endif //ADJLIST_H
