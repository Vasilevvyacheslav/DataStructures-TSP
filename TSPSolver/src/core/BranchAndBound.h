#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include "TSPGraph.h"
#include <vector>
#include <string>
using namespace std;

// Шаг алгоритма для визуализации
struct BnBStep {
    int stepNumber;
    string description;
    vector<vector<double>> currentMatrix;
    double lowerBound;
    vector<int> currentPath;
    int selectedRow;
    int selectedCol;

    BnBStep() : stepNumber(0), lowerBound(0), selectedRow(-1), selectedCol(-1) {}
};

// Результат решения
struct TSPResult {
    vector<int> bestPath;
    double bestCost;
    vector<BnBStep> steps;
    bool solved;

    TSPResult() : bestCost(0), solved(false) {}
};

class BranchAndBound {
private:
    TSPGraph* graph;
    vector<BnBStep> steps;
    int stepCounter;

    // Вспомогательные методы
    vector<vector<double>> copyMatrix(const vector<vector<double>>& matrix);
    double calculateReductionCost(vector<vector<double>>& matrix);
    void reduceMatrix(vector<vector<double>>& matrix, double& cost);
    void findMaxZero(const vector<vector<double>>& matrix, int& row, int& col);

    // Рекурсивный поиск
    void solveRecursive(vector<vector<double>> matrix, vector<int> path,
                        double currentCost, double upperBound, TSPResult& result);

public:
    BranchAndBound(TSPGraph* g);

    // Основной метод
    TSPResult solve();

    // Получить шаги для визуализации
    vector<BnBStep> getSteps() const { return steps; }
};

#endif // BRANCHANDBOUND_H
