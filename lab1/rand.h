#ifndef RAND_H
#define RAND_H

#include <type_traits>
#include <cstdlib>

template <typename T>
T generateRandomEdge(const T& maxValue) {
    if (std::is_arithmetic<T>::value) {
        return rand() % (static_cast<int>(maxValue) + 1);
    } else {
        return T{};
    }
}

#endif //RAND_H
