#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <vector>

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

private:
    void setupUi();

    Ui::MainWindow *ui;
    std::vector<QLabel*> labels;
};

#endif // MAINWINDOW_H
