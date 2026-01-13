// External Libraries
// Boost Mysql headers must be included before the QT header files or they do no compile.

// Project Header Files
#include "TestDBConnection.h"
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"
#include "DataBaseConnectionDialog.h"

// QT Header Files
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <format>
#include <iostream>
#include <string>

DataBaseConnectionDialog::DataBaseConnectionDialog(QWidget *parent)
    : QDialog(parent),
    maxGroupBoxHeight{0},
    maxButtonBoxHeight{0}
{
    setUpDBConnectionDialogUI();
}

DataBaseConnectionDialog::~DataBaseConnectionDialog()
{

}

void DataBaseConnectionDialog::setUpDBConnectionDialogUI()
{
    dbConnectionsDialogLayout = new QVBoxLayout(this);
    dbConnectionsDialogLayout->setObjectName("dbConnectionsDialogLayout");

    dbConnectionsGB = new QGroupBox("MySQL Database Connection Data", this);
    dbConnectionsGB->setObjectName("mysqlConnectionsGB");

    dbConnectionsGB->setLayout(setUpDBConnectionsFormLayout());

    dbConnectionsDialogLayout->addWidget(dbConnectionsGB);

    dbConnectionsDialogLayout->addWidget(setUpButtonBox(), 0 , Qt::AlignHCenter);

    setLayout(dbConnectionsDialogLayout);

    limitDialogRowth();

    adjustSize();

    setWindowTitle("Database Connection Dialog");
}

QFormLayout *DataBaseConnectionDialog::setUpDBConnectionsFormLayout()
{
    dbConnectionsFormLayout = cqtfa_FormLayoutWithPolicy("dbConnectionsFormLayout", dbConnectionsGB);

    mySqlUser = cqtfa_LineEditFixedWidthByCharCount("mySqlUser", dbConnectionsGB, maxQLineEditCharCount);
    mySqlUser->setText(QString::fromStdString(programOptions.mySqlUser));
    dbConnectionsFormLayout->addRow("MySQL User Name:", mySqlUser);

    mySqlPassword = cqtfa_LineEditFixedWidthByCharCount("mySqlPassword", dbConnectionsGB, maxQLineEditCharCount);
    mySqlPassword->setText(QString::fromStdString(programOptions.mySqlPassword));
    dbConnectionsFormLayout->addRow("MySQL User Password:", mySqlPassword);

    mySqlUrl = cqtfa_LineEditFixedWidthByCharCount("mySqlUrl", dbConnectionsGB, maxQLineEditCharCount);
    mySqlUrl->setText(QString::fromStdString(programOptions.mySqlUrl));
    dbConnectionsFormLayout->addRow("MySQL URL:", mySqlUrl);

    mySqlPort = cqtfa_LineEditFixedWidthByCharCount("mySqlPort", dbConnectionsGB, maxQLineEditCharCount);
    mySqlPort->setText(QString::number(programOptions.mySqlPort));
    dbConnectionsFormLayout->addRow("MySQL Port:", mySqlPort);

    mySqlDBName = cqtfa_LineEditFixedWidthByCharCount("mySqlDBName", dbConnectionsGB, maxQLineEditCharCount);
    mySqlDBName->setText(QString::fromStdString(programOptions.mySqlDBName));
    dbConnectionsFormLayout->addRow("MySQL Database Name:", mySqlDBName);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(dbConnectionsFormLayout);


    return dbConnectionsFormLayout;
}

QDialogButtonBox *DataBaseConnectionDialog::setUpButtonBox()
{
    dbConnectionsButtonBox = new QDialogButtonBox(this);

    dbConnectionsButtonBox->setObjectName(QString::fromUtf8("editNoteButtonBox"));
    dbConnectionsButtonBox->setOrientation(Qt::Horizontal);
    dbConnectionsButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    maxButtonBoxHeight = dbConnectionsButtonBox->height() + marginAndSpacing;

    connect(dbConnectionsButtonBox, &QDialogButtonBox::accepted, this, &DataBaseConnectionDialog::on_dbConnectionsButtonBox_accepted);
    connect(dbConnectionsButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return dbConnectionsButtonBox;
}

void DataBaseConnectionDialog::limitDialogRowth()
{
    dbConnectionsGB->setMaximumHeight(maxGroupBoxHeight);
    dbConnectionsGB->setMaximumWidth(cqtfa_getFormLayoutMaxWidth(dbConnectionsFormLayout) + marginAndSpacing);

    dbConnectionsButtonBox->setMaximumHeight(maxButtonBoxHeight);

    int maxDialogWidth = dbConnectionsGB->maximumWidth() + marginAndSpacing;

    setMaximumWidth(maxDialogWidth);

    setMaximumHeight(maxGroupBoxHeight + maxButtonBoxHeight + marginAndSpacing);
}

bool DataBaseConnectionDialog::testConnection()
{
    TestDBConnection dbConnectionTest;

    if (!dbConnectionTest.runTestConnect())
    {
        QString errorMessages = QString::fromStdString(dbConnectionTest.getErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorMessages, QMessageBox::Ok);
        return false;
    }

    return true;
}

bool DataBaseConnectionDialog::validateAndUpdateProgramOption()
{
    bool ok = false;
    QString errorMessages;

    std::string inputUserName = mySqlUser->text().toStdString();
    std::string inputPassword = mySqlPassword->text().toStdString();

    if (inputUserName.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Name\n");
    }

    if (inputPassword.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Password\n");
    }

    programOptions.mySqlPort = mySqlPort->text().toUInt(&ok);
    if (ok == false)
    {
        errorMessages.append("Unable to convert My SQL Port value to unsigned integer.\n");
    }

    if (!errorMessages.isEmpty())
    {
        QMessageBox::critical(nullptr, "Critical Error", errorMessages, QMessageBox::Ok);
        return false;
    }

    programOptions.mySqlUser = inputUserName;
    programOptions.mySqlPassword = inputPassword;
    programOptions.mySqlUrl = mySqlUrl->text().toStdString();
    programOptions.mySqlDBName = mySqlDBName->text().toStdString();

    return ok;
}

void DataBaseConnectionDialog::on_dbConnectionsButtonBox_accepted()
{
    if(validateAndUpdateProgramOption())
    {
        if (testConnection())
        {
            QDialog::accept();
        }
        else
        {

        }
    }
    return;
}
