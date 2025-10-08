#ifndef DATA_H
#define DATA_H

#include <sstream>
#include <type_traits>
#include <vehicle.h>

// Структура для зберігання даних у вершинах графа

template <typename vertexT, typename edgeT>
struct Data
{
public:
    int number {};
    vertexT vertexData;
    edgeT edgeData;

    explicit Data(vertexT vertexData, edgeT edgeData, int number = 0)
        : vertexData(vertexData), edgeData(edgeData), number(number) {}

    explicit Data(edgeT edgeData, int number = 0)
        : vertexData(), edgeData(edgeData), number(number) {}

    Data() : number(-1), vertexData(), edgeData() {}

    // Отримання "ваги" ребра для алгоритмів пошуку шляхів
    double getWeight() const {
        return getWeightImpl(edgeData);
    }

private:
    // для числових типів
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, double>::type
    getWeightImpl(const T& value) const {
        return static_cast<double>(value);
    }

    // для класів (наприклад, BaseVehicle)
    template <typename T>
    typename std::enable_if<!std::is_arithmetic<T>::value, double>::type
    getWeightImpl(const T& value) const {
        return value.getWeight();
    }
};

// getWeight() дозволяє універсально працювати з будь-якими типами даних: числа і власні класи.

// універсальна функція для перетворення edgeData на string
template <typename T>
std::string edgeToString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// спеціалізація для BaseVehicle
template<>
std::string edgeToString<BaseVehicle>(const BaseVehicle& v) {
    return v.toString();
}

// edgeToString() дозволяє виводити ребра у зручному вигляді.


#endif //DATA_H
