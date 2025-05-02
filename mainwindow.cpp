#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    finalLabels.clear();
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
}

void MainWindow::createRandomPattern()
{
    QString matrixText;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int index = row * 10 + col;
            int value = QRandomGenerator::global()->bounded(2);
            labels[index]->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                             .arg(value == 1 ? "blue" : "yellow"));
            currentMatrix[row][col] = value;
        }
    }
    updateMatrixText();
}
void MainWindow::optimizePattern()
{
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

    int bestCost = calculateCost(currentMatrix, finalMatrix);
    qDebug() << "Initial Cost:" << bestCost;
    printMatrix(currentMatrix);

    int iterations = 0;
    const int maxIterations = 1000;
    while (iterations < maxIterations) {
        int bestNeighborCost = bestCost;
        int bestRow = -1, bestCol = -1;

        // Komşu çözümleri değerlendir (her hücreyi tersine çevir)
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                int tempMatrix[10][10];
                memcpy(tempMatrix, currentMatrix, sizeof(currentMatrix));
                tempMatrix[row][col] = 1 - tempMatrix[row][col]; // Hücreyi tersine çevir
                int neighborCost = calculateCost(tempMatrix, finalMatrix);
                if (neighborCost < bestNeighborCost) {
                    bestNeighborCost = neighborCost;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }

        if (bestNeighborCost < bestCost) {
            currentMatrix[bestRow][bestCol] = 1 - currentMatrix[bestRow][bestCol];
            bestCost = bestNeighborCost;
            updateMatrixDisplay();
            qDebug() << "Iteration" << iterations << "Cost:" << bestCost;
            printMatrix(currentMatrix);
        } else {
            break; // Yerel minimuma ulaşıldı
        }
        iterations++;
    }
    qDebug() << "Final Cost:" << bestCost;
}

int MainWindow::calculateCost(int matrix1[10][10], int matrix2[10][10])
{
    int cost = 0;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            cost += abs(matrix1[row][col] - matrix2[row][col]);
        }
    }
    return cost;
}

void MainWindow::updateMatrixDisplay()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int index = row * 10 + col;
            labels[index]->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                             .arg(currentMatrix[row][col] == 1 ? "blue" : "yellow"));
        }
    }
    updateMatrixText();
}

void MainWindow::updateMatrixText()
{
    QString matrixText;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            matrixText += QString("%1 ").arg(currentMatrix[row][col]);
        }
        matrixText += "\n";
    }
    ui->textEditMatrix->setText(matrixText);
}

void MainWindow::printMatrix(int matrix[10][10])
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
