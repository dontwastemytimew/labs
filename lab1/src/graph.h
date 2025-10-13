#ifndef GRAPH_H
#define GRAPH_H

/**
 * @file graph.h
 * @brief Базовий клас графа.
 *
 * Graph визначає інтерфейс для графів, які реалізують
 * списки суміжності або матрицю суміжності.
 *
 * @tparam T Тип вершини графа.
 */

/**
 * @brief Абстрактний базовий клас графа.
 * @tparam T тип вершини графа
 */
template <typename T>
class Graph {
public:
    /** Віртуальний деструктор */
    virtual ~Graph() = default;

    /**
     * @brief Додає вершину у граф.
     * @param v Вершина для додавання.
     */
    virtual void addVertex(const T& v) = 0;

    /**
     * @brief Видаляє вершину з графа.
     * @param v Вершина для видалення.
     */
    virtual void removeVertex(const T& v) = 0;

 /**
   * @brief Додає ребро між вершинами з вказаною вагою.
   * @param v1 Початкова вершина.
   * @param v2 Кінцева вершина.
   * @param weight Вага ребра (за замовчуванням 1.0).
   */
    virtual void addEdge(const T& v1, const T& v2, double weight = 1.0) = 0;

 /**
  * @brief Видаляє ребро між вершинами.
  * @param v1 Початкова вершина.
  * @param v2 Кінцева вершина.
  */
    virtual void removeEdge(const T& v1, const T& v2) = 0;

 /**
  * @brief Перевіряє, чи граф зв’язний.
  * @return true якщо граф зв’язний, false інакше.
  */
    virtual bool isConnected() const = 0;

 /**
    * @brief Обчислює відстань між двома вершинами.
    * @param v1 Початкова вершина.
    * @param v2 Кінцева вершина.
    * @return Вага шляху між вершинами, або -1 якщо шляху немає.
    */
    virtual double distance(const T& v1, const T& v2) const = 0;
};

#endif //GRAPH_H
