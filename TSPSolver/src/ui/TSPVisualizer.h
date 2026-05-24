#ifndef TSPVISUALIZER_H
#define TSPVISUALIZER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFont>
#include <QColor>
#include <cmath>
#include "TSPGraph.h"
#include "BranchAndBound.h"

const double PI = 3.14159265;
const double CENTER_X = 400.0;
const double CENTER_Y = 300.0;
const double RADIUS = 180.0;
const int VERTEX_RADIUS = 25;

class TSPVisualizer {
private:
    QGraphicsScene* m_scene;
    TSPGraph* m_graph;

    QFont m_fontVertex;
    QFont m_fontWeight;
    QFont m_fontRoute;

    QColor m_colorNode;
    QColor m_colorEdge;
    QColor m_colorRoute;
    QColor m_colorTextWeight;

    // Вспомогательные методы отрисовки
    QPointF getVertexPosition(int index, int n);
    void drawEdges();
    void drawRouteEdges(const TSPResult& result);

public:
    TSPVisualizer(QGraphicsScene* scene, TSPGraph* graph);

    void clear();
    void drawGraph();
    void drawRoute(const TSPResult& result);
};

#endif // TSPVISUALIZER_H
