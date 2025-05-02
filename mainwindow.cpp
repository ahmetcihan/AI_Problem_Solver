#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUi();
    setWindowTitle("AI Problem Solver");
    optimizationStep = 0;
    bestNeighborCost = 0;
    bestRow = -1;
    bestCol = -1;
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

    // Initialize timer
    optimizationTimer = new QTimer(this);
    connect(optimizationTimer, &QTimer::timeout, this, &MainWindow::nextIteration);
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

    // Reset optimization state
    optimizationStep = 0;
    bestNeighborCost = bestCost;

    // Start the optimization process with timer
    optimizationTimer->start(ui->spinBoxTickInterval->value());
}
void MainWindow::nextIteration()
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

    int currentCost = calculateCost(currentMatrix, finalMatrix);
    if (optimizationStep == 0) {
        qDebug() << "Initial Cost:" << currentCost;
        printMatrix(currentMatrix);
    }

    // Select a random cell
    int row = QRandomGenerator::global()->bounded(10);
    int col = QRandomGenerator::global()->bounded(10);
    int tempMatrix[10][10];
    memcpy(tempMatrix, currentMatrix, sizeof(currentMatrix));
    tempMatrix[row][col] = 1 - tempMatrix[row][col];
    int newCost = calculateCost(tempMatrix, finalMatrix);

    if (newCost < currentCost) {
        currentMatrix[row][col] = tempMatrix[row][col];
        updateMatrixDisplay();
        qDebug() << "Iteration" << optimizationStep << "Cost:" << newCost;
        printMatrix(currentMatrix);
        bestNeighborCost = newCost;
    } else {
        // If no improvement, still update display to show the attempt
        updateMatrixDisplay();
        qDebug() << "Iteration" << optimizationStep << "Cost (no change):" << currentCost;
        printMatrix(currentMatrix);
    }

    optimizationStep++;
    if (optimizationStep >= 1000 || currentCost == 0) {
        qDebug() << "Final Cost:" << calculateCost(currentMatrix, finalMatrix);
        optimizationTimer->stop();
        return;
    }
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
