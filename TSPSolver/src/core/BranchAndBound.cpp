#include "BranchAndBound.h"
#include <algorithm>
#include <cmath>

// Конструктор
BranchAndBound::BranchAndBound(TSPGraph* g) : graph(g), stepCounter(1) {}

// Копирование матрицы
vector<vector<double>> BranchAndBound::copyMatrix(const vector<vector<double>>& matrix) {
    return matrix;
}

// Расчёт стоимости приведения (сумма минимумов по строкам и столбцам)
double BranchAndBound::calculateReductionCost(vector<vector<double>>& matrix) {
    int n = matrix.size();
    double cost = 0;

    // Приведение по строкам
    for (int i = 0; i < n; i++) {
        double minVal = INF;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        if (minVal < INF && minVal > 0) {
            cost += minVal;
        }
    }

    // Приведение по столбцам
    for (int j = 0; j < n; j++) {
        double minVal = INF;
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        if (minVal < INF && minVal > 0) {
            cost += minVal;
        }
    }

    return cost;
}

// Приведение матрицы
void BranchAndBound::reduceMatrix(vector<vector<double>>& matrix, double& cost) {
    int n = matrix.size();
    cost = 0;

    // Приведение по строкам
    for (int i = 0; i < n; i++) {
        double minVal = INF;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        if (minVal < INF && minVal > 0) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= minVal;
                }
            }
            cost += minVal;
        }
    }

    // Приведение по столбцам
    for (int j = 0; j < n; j++) {
        double minVal = INF;
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        if (minVal < INF && minVal > 0) {
            for (int i = 0; i < n; i++) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= minVal;
                }
            }
            cost += minVal;
        }
    }
}

// Поиск ячейки с максимальной суммой минимумов (правило выбора ребра)
void BranchAndBound::findMaxZero(const vector<vector<double>>& matrix, int& row, int& col) {
    int n = matrix.size();
    double maxSum = -1;
    row = -1;
    col = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) {
                // Ищем минимум в строке (кроме этой ячейки)
                double rowMin = INF;
                for (int k = 0; k < n; k++) {
                    if (k != j && matrix[i][k] < rowMin) {
                        rowMin = matrix[i][k];
                    }
                }

                // Ищем минимум в столбце (кроме этой ячейки)
                double colMin = INF;
                for (int k = 0; k < n; k++) {
                    if (k != i && matrix[k][j] < colMin) {
                        colMin = matrix[k][j];
                    }
                }

                double sum = (rowMin < INF ? rowMin : 0) + (colMin < INF ? colMin : 0);
                if (sum > maxSum) {
                    maxSum = sum;
                    row = i;
                    col = j;
                }
            }
        }
    }
}

// Рекурсивное решение
void BranchAndBound::solveRecursive(vector<vector<double>> matrix, vector<int> path,
                                     double currentCost, double upperBound, TSPResult& result) {
    int n = matrix.size();

    // Создаём шаг для визуализации
    BnBStep step;
    step.stepNumber = stepCounter++;
    step.currentMatrix = matrix;
    step.lowerBound = currentCost;
    step.currentPath = path;

    // Если достигли конца
    if (path.size() == n) {
        // Проверяем, можно ли вернуться в начало
        if (matrix[path.back()][path[0]] < INF) {
            double finalCost = currentCost + matrix[path.back()][path[0]];
            if (finalCost < result.bestCost || result.bestCost == 0) {
                result.bestCost = finalCost;
                result.bestPath = path;
                result.bestPath.push_back(path[0]);  // Замыкаем цикл
                result.solved = true;

                step.description = "Найден маршрут стоимостью: " + to_string(finalCost);
                steps.push_back(step);
            }
        }
        return;
    }

    // Приводим матрицу
    double reductionCost = 0;
    reduceMatrix(matrix, reductionCost);
    currentCost += reductionCost;

    step.description = "Приведение матрицы. Нижняя оценка: " + to_string(currentCost);
    steps.push_back(step);

    // Если нижняя оценка уже больше верхней границы - отсекаем
    if (upperBound > 0 && currentCost >= upperBound) {
        return;
    }

    // Находим лучшую ячейку для ветвления
    int row, col;
    findMaxZero(matrix, row, col);

    if (row == -1) return;  // Нет нулей

    // Ветвление 1: включаем ребро (row, col)
    vector<int> newPath = path;
    newPath.push_back(row);

    vector<vector<double>> newMatrix = matrix;
    newMatrix[row][col] = INF;  // Запрещаем обратное ребро

    // Удаляем строку и столбец (помечаем как INF)
    for (int i = 0; i < n; i++) {
        newMatrix[row][i] = INF;
        newMatrix[i][col] = INF;
    }

    // Рекурсивный вызов
    solveRecursive(newMatrix, newPath, currentCost, result.bestCost, result);

    // Ветвление 2: исключаем ребро (row, col)
    matrix[row][col] = INF;
    solveRecursive(matrix, path, currentCost, upperBound, result);
}

// Основной метод решения
TSPResult BranchAndBound::solve() {
    TSPResult result;
    steps.clear();
    stepCounter = 1;

    if (!graph->isComplete()) {
        result.solved = false;
        return result;
    }

    vector<vector<double>> matrix = graph->getMatrix();
    vector<int> path;

    solveRecursive(matrix, path, 0, 0, result);

    return result;
}
