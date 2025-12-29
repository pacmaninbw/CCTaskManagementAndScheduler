// Project Headers
#include "CommandLineParser.h"


// QT Headers
#include "EditUserProfileDialog.h"
#include "mainwindow.h"

// Standard C++ Header Files
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    progNameStr = QString::fromStdString(programOptions.progName);

    setUpMainWindowUI();

//    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUpMainWindowUI()
{
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    mwLayout = new QVBoxLayout(centralwidget);

    dbConnectionsGB = creatSqlConnectionsGB();
    mwLayout->addWidget(dbConnectionsGB, 0, Qt::AlignHCenter);

    postDbConnectActions = creationPostDbConnectionsBox();
    mwLayout->addWidget(postDbConnectActions, 0, Qt::AlignHCenter);

    resize(mainWindowWidth, mainWindowHeight);

    setCentralWidget(centralwidget);
    setWindowTitle(progNameStr);
}

QGroupBox* MainWindow::creatSqlConnectionsGB()
{
    QGroupBox* mysqlConnectionsGB = new QGroupBox("MySQL Database Connection Data", this);

    dbConnectionsForm = createNamedQTWidget<QFormLayout>("dbConnectionsForm");
    dbConnectionsForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    mySqlUser = createMySqlConnectLineEdit("mySqlUser");
    mySqlUser->setText(QString::fromStdString(programOptions.mySqlUser));
    dbConnectionsForm->addRow("MySQL User Name:", mySqlUser);

    mySqlPassword = createMySqlConnectLineEdit("mySqlPassword");
    mySqlPassword->setText(QString::fromStdString(programOptions.mySqlPassword));
    dbConnectionsForm->addRow("MySQL User Password:", mySqlPassword);

    mySqlUrl = createMySqlConnectLineEdit("mySqlUrl");
    mySqlUrl->setText(QString::fromStdString(programOptions.mySqlUrl));
    dbConnectionsForm->addRow("MySQL URL:", mySqlUrl);

    mySqlPort = createMySqlConnectLineEdit("mySqlPort");
    mySqlPort->setText(QString::number(programOptions.mySqlPort));
    dbConnectionsForm->addRow("MySQL Port:", mySqlPort);

    mySqlDBName = createMySqlConnectLineEdit("mySqlDBName");
    mySqlDBName->setText(QString::fromStdString(programOptions.mySqlDBName));
    dbConnectionsForm->addRow("MySQL Database Name:", mySqlDBName);

    saveDbConnectionData = CreateNamedButton("Save", "saveDbConnectionData");
    dbConnectionsForm->addRow(saveDbConnectionData);

    mysqlConnectionsGB->setLayout(dbConnectionsForm);

    return mysqlConnectionsGB;
}

QGroupBox* MainWindow::creationPostDbConnectionsBox()
{
    QGroupBox* actionButtons = new QGroupBox("Actions After DB Connection", this);

    actionButtonLayout = createNamedQTWidget<QHBoxLayout>("actionButtonLayout");

    actionAddUser = CreateNamedButton("Add New User", "actionAddUser");
    actionButtonLayout->addWidget(actionAddUser);
    connect(actionAddUser, &QPushButton::clicked, this, &MainWindow::handle_actionAddUser_Clicked);


    actionLoginAsUser = CreateNamedButton("Login as User", "actionLoginAsUser");
    actionButtonLayout->addWidget(actionLoginAsUser);

    actionButtons->setLayout(actionButtonLayout);

    return actionButtons;
}

void MainWindow::handle_actionAddUser_Clicked()
{
    EditUserProfileDialog editUser(this);

    editUser.exec();
}

int MainWindow::getLabelWidth(QLabel *lab)
{
    QFont currentFont = lab->font();
    QFontMetrics fm(currentFont);
    return fm.horizontalAdvance(lab->text());
}

QLabel *MainWindow::createNamedLabel(const char *labText, const char *labName)
{
    QLabel* newLabel = createNameQTWidgetWithText<QLabel>(labText, labName, centralwidget);
    newLabel->setStyleSheet(generateWidthAndHeightStyleString(getLabelWidth(newLabel), labelHeight));
    newLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    return newLabel;
}

QLineEdit* MainWindow::createMySqlConnectLineEdit(const char* fieldName)
{
    QLineEdit* mySqlLineEdit = createNamedQTWidget<QLineEdit>(fieldName, this);
    mySqlLineEdit->setStyleSheet("width: 200px;");
    mySqlLineEdit->setMaxLength(20);

    return mySqlLineEdit;
}

QPushButton* MainWindow::CreateNamedButton(const char* buttonText, const char* buttonName)
{
    QPushButton* newButton = createNameQTWidgetWithText<QPushButton>(buttonText, buttonName, centralwidget);
    newButton->setStyleSheet(generateWidthAndHeightStyleString(newButton->width(), buttonHeight));

    return newButton;
}

QString MainWindow::generateWidthAndHeightStyleString(const int width, const int height)
{
    QString widthAndHeightStyleString("width: ");
    widthAndHeightStyleString += QString::number(width) + "; height:";
    widthAndHeightStyleString += QString::number(height) += ";";

    return widthAndHeightStyleString;
}


