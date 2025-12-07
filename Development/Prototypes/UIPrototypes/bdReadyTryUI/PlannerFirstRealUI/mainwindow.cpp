// Project Headers
#include "CommandLineParser.h"

// QT Headers
#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Standard C++ Header Files

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
