#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <vector>
#include <QRandomGenerator>
#include <array>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>

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
    void nextGeneration();
    void stopOptimization();

private:
    void setupUi();
    double calculateFitness(const std::array<std::array<int, 10>, 10>& matrix);
    void createPopulation();
    void updateBestIndividual();
    void updateMatrixDisplay();
    void printMatrix(const std::array<std::array<int, 10>, 10>& matrix);

    Ui::MainWindow *ui;
    std::vector<QLabel*> labels;
    std::vector<QLabel*> finalLabels;
    QTimer *optimizationTimer;
    std::vector<std::array<std::array<int, 10>, 10>> population;
    std::array<std::array<int, 10>, 10> bestIndividual;
    int generation;
    int populationSize;
    int maxGenerations;
    qint64 startTime;

    std::array<std::array<int, 10>, 10> currentMatrix = {};
    std::array<std::array<int, 10>, 10> finalMatrix = {{
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
    }};
};

#endif // MAINWINDOW_H
