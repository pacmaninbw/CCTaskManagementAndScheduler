#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project Headers
#include "createNamedQTWidget.h"

// QT Headers
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QRect>
#include <QStatusBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

signals:

private slots:
    void handle_actionAddUser_Clicked();

private:
    void setUpMainWindowUI();
    QGroupBox* creatSqlConnectionsGB();
    QGroupBox* creationPostDbConnectionsBox();
    int getLabelWidth(QLabel* lab);
    QLabel* createNamedLabel(const char* labText, const char* labName);
    QLineEdit* createMySqlConnectLineEdit(const char* fieldName);
    QPushButton* CreateNamedButton(const char* buttonText, const char* buttonName);
    QString generateWidthAndHeightStyleString(const int width, const int height);

    QString progNameStr;

/*
 * Size and positioning constants.
 */
    const int mainWindowWidth = 800;
    const int mainWindowHeight = 500;
    const int maxOjectWidth = static_cast<int>(mainWindowWidth * 0.8);
    const int labelHeight = 17;
    const int buttonHeight = 25;

    QWidget* centralwidget;
    QVBoxLayout* mwLayout;
    QGroupBox* dbConnectionsGB;
    QFormLayout* dbConnectionsForm;
    QLineEdit* mySqlUser;
    QLineEdit* mySqlPassword;
    QLineEdit* mySqlUrl;
    QLineEdit* mySqlPort;
    QLineEdit* mySqlDBName;
    QPushButton* saveDbConnectionData;

    QGroupBox* postDbConnectActions;
    QHBoxLayout* actionButtonLayout;
    QPushButton* actionAddUser;
    QPushButton* actionLoginAsUser;
};
#endif // MAINWINDOW_H
