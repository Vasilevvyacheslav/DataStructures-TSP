#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graph = new TSPGraph(4);
    updateTableFromGraph();

    // Настройка сцены
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->setScene(scene);

    // Создаём визуализатор
    visualizer = new TSPVisualizer(scene, graph);

    // Подключение кнопок
    connect(ui->btnAddVertex, &QPushButton::clicked, this, &MainWindow::onBtnAddVertex);
    connect(ui->btnRemoveVertex, &QPushButton::clicked, this, &MainWindow::onBtnRemoveVertex);
    connect(ui->btnClearMatrix, &QPushButton::clicked, this, &MainWindow::onBtnClearMatrix);
    connect(ui->btnSolve, &QPushButton::clicked, this, &MainWindow::onBtnSolve);
    connect(ui->btnShowRoute, &QPushButton::clicked, this, &MainWindow::onBtnShowRoute);
    connect(ui->tableMatrix, &QTableWidget::itemChanged, this, &MainWindow::onTableItemChanged);
    ui->textOutput->appendPlainText("=== TSP Solver готов к работе ===");
    ui->textOutput->appendPlainText("Введите веса рёбер в матрицу смежности");
}

MainWindow::~MainWindow() {
    delete visualizer;
    delete graph;
    delete ui;
}

// Синхронизация: Graph -> Table
void MainWindow::updateTableFromGraph() {
    int n = graph->getSize();
    ui->tableMatrix->setRowCount(n);
    ui->tableMatrix->setColumnCount(n);

    // Устанавливаем заголовки 0, 1, 2... чтобы совпадали с графом
    for (int i = 0; i < n; ++i) {
        ui->tableMatrix->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
        ui->tableMatrix->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                QTableWidgetItem* item = new QTableWidgetItem("0");
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableMatrix->setItem(i, j, item);
            } else {
                double weight = graph->getEdge(i, j);
                QTableWidgetItem* item;
                if (weight >= INF) {
                    item = new QTableWidgetItem("");
                } else {
                    item = new QTableWidgetItem(QString::number(weight, 'g', 6));
                }
                ui->tableMatrix->setItem(i, j, item);
            }
        }
    }
}

// Синхронизация: Table -> Graph
void MainWindow::updateGraphFromTable() {
    int n = ui->tableMatrix->rowCount();
    graph->setSize(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph->setEdge(i, j, 0);
            } else {
                QTableWidgetItem* item = ui->tableMatrix->item(i, j);
                if (item && !item->text().isEmpty()) {
                    bool ok;
                    double weight = item->text().toDouble(&ok);
                    if (ok && weight > 0) {
                        graph->setEdge(i, j, weight);
                    } else {
                        graph->setEdge(i, j, INF);
                    }
                } else {
                    graph->setEdge(i, j, INF);
                }
            }
        }
    }
}

// Добавление вершины
void MainWindow::onBtnAddVertex() {
    int currentSize = graph->getSize();
    if (currentSize >= 10) {
        QMessageBox::warning(this, "Ошибка", "Максимум 10 вершин!");
        return;
    }

    graph->setSize(currentSize + 1);
    updateTableFromGraph();
    ui->textOutput->appendPlainText("Добавлена вершина " + QString::number(currentSize));
}

// Удаление вершины
void MainWindow::onBtnRemoveVertex() {
    int currentSize = graph->getSize();
    if (currentSize <= 2) {
        QMessageBox::warning(this, "Ошибка", "Минимум 3 вершины!");
        return;
    }

    graph->setSize(currentSize - 1);
    updateTableFromGraph();
    ui->textOutput->appendPlainText("Удалена вершина " + QString::number(currentSize - 1));
}

// Очистка матрицы
void MainWindow::onBtnClearMatrix() {
    int n = graph->getSize();
    graph->setSize(n);
    updateTableFromGraph();
    ui->textOutput->appendPlainText("Матрица очищена");
}

// Решение TSP
void MainWindow::onBtnSolve() {
    updateGraphFromTable();

    if (!graph->isComplete()) {
        QMessageBox::warning(this, "Ошибка",
            "Граф должен быть полным!\nЗаполните все рёбра (кроме диагонали).");
        return;
    }

    ui->textOutput->appendPlainText("\n=== Запуск алгоритма ветвей и границ ===");
    ui->textOutput->appendPlainText("Количество вершин: " + QString::number(graph->getSize()));

    BranchAndBound bab(graph);
    currentResult = bab.solve();

    if (currentResult.solved) {
        ui->textOutput->appendPlainText("Найден оптимальный маршрут!");
        ui->textOutput->appendPlainText(
            "Стоимость: " + QString::number(currentResult.bestCost, 'f', 2));

        QString pathStr;
        for (int v : currentResult.bestPath) {
            pathStr += QString::number(v) + " -> ";
        }
        pathStr.chop(4);
        ui->textOutput->appendPlainText("Маршрут: " + pathStr);

        ui->textOutput->appendPlainText("\nНажмите 'Показать маршрут' для визуализации");
    } else {
        ui->textOutput->appendPlainText("Маршрут не найден!");
        ui->textOutput->appendPlainText("Проверьте корректность матрицы");
    }
}

// Показать маршрут
void MainWindow::onBtnShowRoute() {
    if (!currentResult.solved) {
        QMessageBox::warning(this, "Ошибка", "Сначала решите задачу!");
        return;
    }

    visualizer->drawRoute(currentResult);
    ui->textOutput->appendPlainText("Маршрут отрисован!");
}

// Автоматическая синхронизация симметричных ячеек
void MainWindow::onTableItemChanged(QTableWidgetItem* item) {
    if (!item) return;

    int row = item->row();
    int col = item->column();

    // Пропускаем диагональ
    if (row == col) return;

    QString text = item->text();

    // Обновляем симметричную ячейку [col][row]
    QTableWidgetItem* symmetricItem = ui->tableMatrix->item(col, row);
    if (symmetricItem && symmetricItem != item) {
        // Отключаем сигнал на время изменения, чтобы избежать бесконечного цикла
        disconnect(ui->tableMatrix, &QTableWidget::itemChanged,
                   this, &MainWindow::onTableItemChanged);
        symmetricItem->setText(text);
        connect(ui->tableMatrix, &QTableWidget::itemChanged,
                this, &MainWindow::onTableItemChanged);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    // Пересчитываем масштаб при изменении размера окна
    if (scene && !scene->items().isEmpty()) {
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
