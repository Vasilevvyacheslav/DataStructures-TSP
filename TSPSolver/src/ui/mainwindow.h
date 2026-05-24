#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QMessageBox>
#include "TSPGraph.h"
#include "BranchAndBound.h"
#include "TSPVisualizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    TSPGraph* graph;
    TSPResult currentResult;
    QGraphicsScene* scene;
    TSPVisualizer* visualizer;

    void updateTableFromGraph();
    void updateGraphFromTable();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBtnAddVertex();
    void onBtnRemoveVertex();
    void onBtnClearMatrix();
    void onBtnSolve();
    void onBtnShowRoute();
    void onTableItemChanged(QTableWidgetItem* item);

protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // MAINWINDOW_H
