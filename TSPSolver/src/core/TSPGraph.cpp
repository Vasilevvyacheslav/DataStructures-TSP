#include "TSPGraph.h"

// Конструктор
TSPGraph::TSPGraph(int size) : n(size) {
    matrix.resize(n, vector<double>(n, INF));
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;  // Диагональ = 0
    }
}

// Установка размера матрицы
void TSPGraph::setSize(int size) {
    n = size;
    matrix.resize(n, vector<double>(n, INF));
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
    }
}

// Установка ребра
void TSPGraph::setEdge(int from, int to, double weight) {
    if (from >= 0 && from < n && to >= 0 && to < n && from != to) {
        matrix[from][to] = weight;
    }
}

// Получение веса ребра
double TSPGraph::getEdge(int from, int to) const {
    if (from >= 0 && from < n && to >= 0 && to < n) {
        return matrix[from][to];
    }
    return INF;
}

// Проверка наличия ребра
bool TSPGraph::hasEdge(int from, int to) const {
    return getEdge(from, to) < INF;
}

// Удаление ребра
void TSPGraph::removeEdge(int from, int to) {
    if (from >= 0 && from < n && to >= 0 && to < n && from != to) {
        matrix[from][to] = INF;
    }
}

// Проверка на полноту графа
bool TSPGraph::isComplete() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && matrix[i][j] >= INF) {
                return false;
            }
        }
    }
    return true;
}

// Получение всех рёбер
vector<Edge> TSPGraph::getAllEdges() const {
    vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && matrix[i][j] < INF) {
                edges.push_back(Edge(i, j, matrix[i][j]));
            }
        }
    }
    return edges;
}
