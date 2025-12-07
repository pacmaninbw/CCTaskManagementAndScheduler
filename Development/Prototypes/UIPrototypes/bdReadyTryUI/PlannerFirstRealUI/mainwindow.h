#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project Headers
#include "CommandLineParser.h"

// QT Headers
#include <QMainWindow>

// Standard C++ Header Files

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
