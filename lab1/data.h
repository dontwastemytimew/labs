#ifndef DATA_H
#define DATA_H

// Структура для зберігання даних у вершинах графа

template <typename vertexT, typename edgeT>
struct Data
{
public:
    int number {};           // номер вершини (для списків суміжності)
    vertexT vertexData;      // дані вершини
    edgeT edgeData;          // дані ребра

    explicit Data(vertexT vertexData, edgeT edgeData, int number = 0)
        : vertexData(vertexData), edgeData(edgeData), number(number) {}

    explicit Data(edgeT edgeData, int number = 0)
        : vertexData(), edgeData(edgeData), number(number) {}

    Data() : number(-1), vertexData(), edgeData() {}

    // Для отримання ваги edgeData
    double getWeight() const {
        return getWeightImpl(edgeData);
    }

private:
    // Для числових типів
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, double>::type
    getWeightImpl(const T& value) const {
        return static_cast<double>(value);
    }

    // Для класів
    template <typename T>
    typename std::enable_if<!std::is_arithmetic<T>::value, double>::type
    getWeightImpl(const T& value) const {
        return value.getWeight();
    }
};


#endif //DATA_H
