#include "TSPVisualizer.h"
#include <QGraphicsView>
#include <algorithm>
#include <cmath>

TSPVisualizer::TSPVisualizer(QGraphicsScene* scene, TSPGraph* graph)
    : m_scene(scene), m_graph(graph)
{
    m_fontVertex = QFont("Arial", 14, QFont::Bold);
    m_fontWeight = QFont("Consolas", 11, QFont::Bold);
    m_fontRoute = QFont("Consolas", 14, QFont::Bold);

    m_colorNode = QColor(50, 50, 50);
    m_colorEdge = Qt::white;
    m_colorRoute = Qt::green;
    m_colorTextWeight = QColor(255, 215, 0);
}

void TSPVisualizer::clear() {
    if (m_scene) m_scene->clear();
}

QPointF TSPVisualizer::getVertexPosition(int index, int n) {
    double angle = (2 * PI * index) / n - PI / 2;
    return QPointF(CENTER_X + RADIUS * cos(angle), CENTER_Y + RADIUS * sin(angle));
}

// Вспомогательная функция для рисования текста веса со смещением
void drawWeightText(QGraphicsScene* scene, const QPointF& pos1, const QPointF& pos2,
                    const QString& text, const QFont& font, const QColor& color, int zValue, int offsetBase) {
    QPointF mid = (pos1 + pos2) / 2;

    double dx = pos2.x() - pos1.x();
    double dy = pos2.y() - pos1.y();
    double len = std::sqrt(dx*dx + dy*dy);

    if (len > 0) {
        double nx = -dy / len;
        double ny = dx / len;
        int offset = offsetBase;
        mid += QPointF(nx * offset, ny * offset);
    }

    QGraphicsTextItem* textItem = scene->addText(text);
    textItem->setFont(font);
    textItem->setDefaultTextColor(color);
    textItem->setZValue(zValue);

    QRectF rect = textItem->boundingRect();
    textItem->setPos(mid.x() - rect.width()/2, mid.y() - rect.height()/2);
}

void TSPVisualizer::drawEdges() {
    if (!m_graph) return;
    int n = m_graph->getSize();
    if (n == 0) return;

    for (int i = 0; i < n; ++i) {
        QPointF pos1 = getVertexPosition(i, n);

        for (int j = i + 1; j < n; ++j) {
            double weight = m_graph->getEdge(i, j);
            if (weight < INF) {
                QPointF pos2 = getVertexPosition(j, n);

                // Белая линия (самый нижний слой)
                QGraphicsLineItem* line = m_scene->addLine(pos1.x(), pos1.y(), pos2.x(), pos2.y(), QPen(m_colorEdge, 1));
                line->setZValue(0);

                // веса(жёлтого цвета)
                drawWeightText(m_scene, pos1, pos2, QString::number(weight, 'g', 6),
                              m_fontWeight, m_colorTextWeight, 1, ((i + j) % 2 != 0) ? -12 : 12);
            }
        }
    }
}

void TSPVisualizer::drawRouteEdges(const TSPResult& result) {
    for (size_t i = 0; i < result.bestPath.size() - 1; i++) {
        int from = result.bestPath[i];
        int to = result.bestPath[i + 1];

        QPointF pos1 = getVertexPosition(from, m_graph->getSize());
        QPointF pos2 = getVertexPosition(to, m_graph->getSize());

        // Зелёная линия (слой между обычными линиями и узлами)
        QGraphicsLineItem* line = m_scene->addLine(pos1.x(), pos1.y(), pos2.x(), pos2.y(), QPen(m_colorRoute, 4));
        line->setZValue(5);
    }
}

void TSPVisualizer::drawGraph() {
    if (!m_scene || !m_graph) return;
    m_scene->clear();
    m_scene->setSceneRect(0, 0, 800, 600);

    int n = m_graph->getSize();
    if (n == 0) return;

    // Обычные рёбра и веса
    drawEdges();

    // Узлы (поверх всех линий)
    for (int i = 0; i < n; ++i) {
        QPointF pos = getVertexPosition(i, n);
        int r = VERTEX_RADIUS;

        // Круг
        QGraphicsItem* node = m_scene->addEllipse(pos.x() - r, pos.y() - r, 2*r, 2*r,
                                                   QPen(Qt::white, 2), QBrush(m_colorNode));
        node->setZValue(10);

        // Номер вершины
        QGraphicsTextItem* text = m_scene->addText(QString::number(i));
        text->setFont(m_fontVertex);
        text->setDefaultTextColor(Qt::white);
        text->setZValue(11);

        QRectF rect = text->boundingRect();
        text->setPos(pos.x() - rect.width()/2, pos.y() - rect.height()/2);
    }
}

void TSPVisualizer::drawRoute(const TSPResult& result) {
    if (!m_scene || !m_graph || !result.solved) return;

    clear();
    m_scene->setSceneRect(0, 0, 800, 600);

    int n = m_graph->getSize();
    if (n == 0) return;

    // Обычные рёбра и золотые веса
    drawEdges();

    // Узлы
    for (int i = 0; i < n; ++i) {
        QPointF pos = getVertexPosition(i, n);
        int r = VERTEX_RADIUS;

        QGraphicsItem* node = m_scene->addEllipse(pos.x() - r, pos.y() - r, 2*r, 2*r,
                                                   QPen(Qt::white, 2), QBrush(m_colorNode));
        node->setZValue(10);

        QGraphicsTextItem* text = m_scene->addText(QString::number(i));
        text->setFont(m_fontVertex);
        text->setDefaultTextColor(Qt::white);
        text->setZValue(11);

        QRectF rect = text->boundingRect();
        text->setPos(pos.x() - rect.width()/2, pos.y() - rect.height()/2);
    }

    // Зелёные рёбра маршрута и белые веса
    drawRouteEdges(result);

    // Масштабирование
    auto views = m_scene->views();
    if (!views.isEmpty()) {
        views.first()->resetTransform();
        views.first()->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
