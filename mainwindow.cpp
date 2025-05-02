#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QDebug>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , generation(0)
{
    ui->setupUi(this);
    setupUi();
    setWindowTitle("AI Problem Solver");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    labels.clear();
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QLabel *label = new QLabel(ui->widgetRandomPattern);
            label->setFixedSize(30, 30);
            label->setStyleSheet("background-color: yellow; border: 1px solid black;");
            label->move(col * 30, row * 30);
            labels.push_back(label);
        }
    }

    connect(ui->pushButton_create_random_pattern, &QPushButton::clicked, this, &MainWindow::createRandomPattern);
    connect(ui->pushButton_start_optimization, &QPushButton::clicked, this, &MainWindow::optimizePattern);
    connect(ui->pushButton_stop_optimization, &QPushButton::clicked, this, &MainWindow::stopOptimization);

    finalLabels.clear();
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QLabel *label = new QLabel(ui->widgetFinalPattern);
            label->setFixedSize(30, 30);
            label->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                     .arg(finalMatrix[row][col] == 1 ? "blue" : "yellow"));
            label->move(col * 30, row * 30);
            finalLabels.push_back(label);
        }
    }

    QString finalMatrixText;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            finalMatrixText += QString("%1 ").arg(finalMatrix[row][col]);
        }
        finalMatrixText += "\n";
    }
    ui->textEditFinalMatrix->setText(finalMatrixText);

    optimizationTimer = new QTimer(this);
    connect(optimizationTimer, &QTimer::timeout, this, &MainWindow::nextGeneration);
}

void MainWindow::stopOptimization()
{
    optimizationTimer->stop();
    qDebug() << "Optimization stopped manually. Final Cost:" << (100 - calculateFitness(bestIndividual) * 100);
    printMatrix(bestIndividual);
}

void MainWindow::createRandomPattern()
{
    population.clear();
    populationSize = ui->spinBox_population_size->value();
    for (int i = 0; i < populationSize; ++i) {
        std::array<std::array<int, 10>, 10> matrix;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                matrix[row][col] = QRandomGenerator::global()->bounded(2);
            }
        }
        population.push_back(matrix);
    }
    updateBestIndividual();
    updateMatrixDisplay();
}

void MainWindow::optimizePattern()
{
    populationSize = ui->spinBox_population_size->value();
    maxGenerations = ui->spinBox_max_generations->value();
    generation = 0;
    if (population.empty()) {
        createPopulation();
    }
    optimizationTimer->start(ui->spinBoxTickInterval->value());
}

void MainWindow::nextGeneration()
{
    if (generation >= maxGenerations) {
        qDebug() << "Max generations reached. Final Cost:" << (100 - calculateFitness(bestIndividual) * 100);
        optimizationTimer->stop();
        return;
    }

    // Step 3: Calculate cost for all individuals
    std::vector<std::pair<double, std::array<std::array<int, 10>, 10>>> fitnessPopulation;
    for (const auto& individual : population) {
        double fitness = calculateFitness(individual);
        fitnessPopulation.emplace_back(fitness, individual);
    }

    // Step 4: Select the best individuals (10% of populationSize)
    std::sort(fitnessPopulation.begin(), fitnessPopulation.end(), [](const auto& a, const auto& b) {
        return a.first > b.first; // Sort by fitness (highest first)
    });
    int eliteSize = populationSize * 0.10; // 10% of population
    std::vector<std::array<std::array<int, 10>, 10>> elite;
    for (int i = 0; i < std::min(eliteSize, static_cast<int>(populationSize)); ++i) {
        elite.push_back(fitnessPopulation[i].second);
    }

    // Step 5: Create new population
    population.clear();
    // Add elite individuals (10%)
    for (const auto& individual : elite) {
        population.push_back(individual);
    }

    // Step 6: Generate new individuals
    int crossoverIndividuals = populationSize * 0.20; // 20% via crossover
    int mutationIndividuals = populationSize * 0.20; // 20% via mutation
    int randomIndividuals = populationSize * 0.50; // 50% completely random

    // Generate individuals via crossover (20%)
    for (int i = 0; i < crossoverIndividuals; ++i) {
        // Select two different parents from elite
        int parent1Idx = QRandomGenerator::global()->bounded(elite.size());
        int parent2Idx;
        do {
            parent2Idx = QRandomGenerator::global()->bounded(elite.size());
        } while (parent2Idx == parent1Idx);
        std::array<std::array<int, 10>, 10> parent1 = elite[parent1Idx];
        std::array<std::array<int, 10>, 10> parent2 = elite[parent2Idx];
        std::array<std::array<int, 10>, 10> child;

        // Uniform Crossover
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                child[row][col] = QRandomGenerator::global()->bounded(2) ? parent1[row][col] : parent2[row][col];
            }
        }

        population.push_back(child);
    }
    // Generate individuals via mutation (20%)
    for (int i = 0; i < mutationIndividuals; ++i) {
        // Select one parent from elite
        int parentIdx = QRandomGenerator::global()->bounded(elite.size());
        std::array<std::array<int, 10>, 10> child = elite[parentIdx]; // Copy the parent

        // Mutation with 15% rate
        double mutationRate = 0.25;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (QRandomGenerator::global()->bounded(1.0) < mutationRate) {
                    child[row][col] = 1 - child[row][col];
                }
            }
        }

        population.push_back(child);
    }

    // Generate completely random individuals (50%)
    for (int i = 0; i < randomIndividuals; ++i) {
        std::array<std::array<int, 10>, 10> randomChild;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                randomChild[row][col] = QRandomGenerator::global()->bounded(2); // 0 or 1
            }
        }
        population.push_back(randomChild);
    }

    updateBestIndividual();
    updateMatrixDisplay();

    qDebug() << "Generation" << generation << "Best Cost:" << (100 - calculateFitness(bestIndividual) * 100);
    printMatrix(bestIndividual);

    generation++;
    if (calculateFitness(bestIndividual) == 1.0) { // Perfect match
        qDebug() << "Perfect match found. Final Cost: 0";
        optimizationTimer->stop();
        return;
    }
}
void MainWindow::createPopulation()
{
    population.clear();
    populationSize = ui->spinBox_population_size->value();
    for (int i = 0; i < populationSize; ++i) {
        std::array<std::array<int, 10>, 10> matrix;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                matrix[row][col] = QRandomGenerator::global()->bounded(2);
            }
        }
        population.push_back(matrix);
    }
    updateBestIndividual();
}

void MainWindow::selection()
{
    // No separate selection, handled in nextGeneration
}

void MainWindow::crossover()
{
    // No separate crossover, handled in nextGeneration
}

void MainWindow::mutation()
{
    // No separate mutation, handled in nextGeneration
}

void MainWindow::updateBestIndividual()
{
    double bestFitness = 0.0;
    for (const auto& individual : population) {
        double fitness = calculateFitness(individual);
        if (fitness > bestFitness) {
            bestFitness = fitness;
            bestIndividual = individual;
        }
    }
}

double MainWindow::calculateFitness(const std::array<std::array<int, 10>, 10>& matrix)
{
    int cost = 0;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            cost += abs(matrix[row][col] - finalMatrix[row][col]);
        }
    }
    return cost == 0 ? 1.0 : 1.0 - (static_cast<double>(cost) / 100.0); // Normalized fitness (0 to 1)
}

void MainWindow::updateMatrixDisplay()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int index = row * 10 + col;
            labels[index]->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                             .arg(bestIndividual[row][col] == 1 ? "blue" : "yellow"));
        }
    }
    updateMatrixText();
}

void MainWindow::updateMatrixText()
{
    QString matrixText;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            matrixText += QString("%1 ").arg(bestIndividual[row][col]);
        }
        matrixText += "\n";
    }
    ui->textEditMatrix->setText(matrixText);
}

void MainWindow::printMatrix(const std::array<std::array<int, 10>, 10>& matrix)
{
    QString matrixStr;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            matrixStr += QString("%1 ").arg(matrix[row][col]);
        }
        matrixStr += "\n";
    }
    qDebug() << matrixStr;
}
