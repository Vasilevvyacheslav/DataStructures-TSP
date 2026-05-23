#ifndef TSPGRAPH_H
#define TSPGRAPH_H

#include <vector>
#include <string>
#include <limits>
using namespace std;

const double INF = 10000;

struct Edge {
    int from;
    int to;
    double weight;

    Edge(int f, int t, double w) : from(f), to(t), weight(w) {}
};

class TSPGraph {
private:
    int n;  // Количество вершин
    vector<vector<double>> matrix;  // Матрица смежности

public:
    TSPGraph(int size = 0);

    // Основные операции
    void setSize(int size);
    int getSize() const { return n; }

    void setEdge(int from, int to, double weight);
    double getEdge(int from, int to) const;
    bool hasEdge(int from, int to) const;
    void removeEdge(int from, int to);

    // Получение матрицы
    vector<vector<double>> getMatrix() const { return matrix; }

    // Проверка на полноту графа
    bool isComplete() const;

    // Получение всех рёбер
    vector<Edge> getAllEdges() const;
};

#endif // TSPGRAPH_H
