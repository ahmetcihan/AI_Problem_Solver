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

    // 10x10 ızgara için QLabel'lar oluştur ve gülen yüz desenini yerleştir
    std::vector<QLabel*> finalLabels;
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
            label->setFixedSize(30, 30); // Her kare 40x40 piksel
            label->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                     .arg(finalMatrix[row][col] == 1 ? "blue" : "yellow"));
            label->move(col * 30, row * 30);
            finalLabels.push_back(label);
        }
    }
    // Final matrisi textEditFinalMatrix'e yazdır
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
            int value = QRandomGenerator::global()->bounded(2); // 0 veya 1 rastgele
            labels[index]->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                             .arg(value == 1 ? "blue" : "yellow"));
            matrixText += QString("%1 ").arg(value);
        }
        matrixText += "\n";
    }
    ui->textEditMatrix->setText(matrixText);
}
