#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <vector>
#include <QRandomGenerator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void createRandomPattern();
    void optimizePattern();
    void nextIteration();
    void stopOptimization();

private:
    void setupUi();

    Ui::MainWindow *ui;
    std::vector<QLabel*> labels;
    std::vector<QLabel*> finalLabels;
    QTimer *optimizationTimer;

    int currentMatrix[10][10] = {};
    int optimizationStep;
    int bestNeighborCost;
    int bestRow;
    int bestCol;
    int finalMatrix[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int calculateCost(int matrix1[10][10], int matrix2[10][10]);
    void updateMatrixDisplay();
    void updateMatrixText();
    void printMatrix(int matrix[10][10]);

};

#endif // MAINWINDOW_H
