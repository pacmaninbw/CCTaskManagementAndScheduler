// External Libraries
// Boost Mysql headers must be included before the QT header files or they do no compile.

// Project Header Files
#include "TestDBConnection.h"
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"
#include "DataBaseConnectionDialog.h"

// QT Header Files
#include <QByteArray>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QtGlobal>
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
    checkEnvironmentVariables();

    setUpDBConnectionDialogUI();
}

DataBaseConnectionDialog::~DataBaseConnectionDialog()
{

}

void DataBaseConnectionDialog::setUpDBConnectionDialogUI()
{
    m_qt_DialogLayout = new QVBoxLayout(this);
    m_qt_DialogLayout->setObjectName("m_qt_DialogLayout");

    m_qt_ConnectionsGB = new QGroupBox("MySQL Database Connection Data", this);
    m_qt_ConnectionsGB->setObjectName("mysqlConnectionsGB");

    m_qt_ConnectionsGB->setLayout(setUpDBConnectionsFormLayout());

    m_qt_DialogLayout->addWidget(m_qt_ConnectionsGB);

    m_qt_DialogLayout->addWidget(setUpOptionsGB());
    
    m_qt_DialogLayout->addWidget(setUpButtonBox(), 0 , Qt::AlignHCenter);

    setLayout(m_qt_DialogLayout);

    limitDialogRowth();

    adjustSize();

    setWindowTitle("Database Connection Dialog");
}

QFormLayout *DataBaseConnectionDialog::setUpDBConnectionsFormLayout()
{
    m_qt_GBFormLayout = cqtfa_FormLayoutWithPolicy("m_qt_GBFormLayout", m_qt_ConnectionsGB);

    m_qt_MySqlUser = cqtfa_LineEditFixedWidthByCharCount("m_qt_MySqlUser", m_qt_ConnectionsGB, maxQLineEditCharCount);
    m_qt_MySqlUser->setText(QString::fromStdString(programOptions.mySqlUser));
    m_qt_GBFormLayout->addRow("MySQL User Name:", m_qt_MySqlUser);

    m_qt_MySqlPassword = cqtfa_LineEditFixedWidthByCharCount("m_qt_MySqlPassword", m_qt_ConnectionsGB, maxQLineEditCharCount);
    m_qt_MySqlPassword->setText(QString::fromStdString(programOptions.mySqlPassword));
    m_qt_GBFormLayout->addRow("MySQL User Password:", m_qt_MySqlPassword);

    m_qt_MySqlUrl = cqtfa_LineEditFixedWidthByCharCount("m_qt_MySqlUrl", m_qt_ConnectionsGB, maxQLineEditCharCount);
    m_qt_MySqlUrl->setText(QString::fromStdString(programOptions.mySqlUrl));
    m_qt_GBFormLayout->addRow("MySQL URL:", m_qt_MySqlUrl);

    m_qt_MySqlPort = cqtfa_LineEditFixedWidthByCharCount("m_qt_MySqlPort", m_qt_ConnectionsGB, maxQLineEditCharCount);
    m_qt_MySqlPort->setText(QString::number(programOptions.mySqlPort));
    m_qt_GBFormLayout->addRow("MySQL Port:", m_qt_MySqlPort);

    m_qt_MySqlDBName = cqtfa_LineEditFixedWidthByCharCount("m_qt_MySqlDBName", m_qt_ConnectionsGB, maxQLineEditCharCount);
    m_qt_MySqlDBName->setText(QString::fromStdString(programOptions.mySqlDBName));
    m_qt_GBFormLayout->addRow("MySQL Database Name:", m_qt_MySqlDBName);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_qt_GBFormLayout);


    return m_qt_GBFormLayout;
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
    m_qt_ConnectionsGB->setMaximumHeight(maxGroupBoxHeight);
    m_qt_ConnectionsGB->setMaximumWidth(cqtfa_getFormLayoutMaxWidth(m_qt_GBFormLayout) + marginAndSpacing);

    dbConnectionsButtonBox->setMaximumHeight(maxButtonBoxHeight);

    int maxDialogWidth = m_qt_ConnectionsGB->maximumWidth() + marginAndSpacing;

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

    std::string inputUserName = m_qt_MySqlUser->text().toStdString();
    std::string inputPassword = m_qt_MySqlPassword->text().toStdString();

    if (inputUserName.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Name\n");
    }

    if (inputPassword.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Password\n");
    }

    programOptions.mySqlPort = m_qt_MySqlPort->text().toUInt(&ok);
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
    programOptions.mySqlUrl = m_qt_MySqlUrl->text().toStdString();
    programOptions.mySqlDBName = m_qt_MySqlDBName->text().toStdString();
    programOptions.verboseOutput = verboseOutput->isChecked();
    programOptions.runSelfTest = runSelftTest->isChecked();
    programOptions.forceErrors = forceErrors->isChecked();
    programOptions.forceExceptions = forceExceptions->isChecked();

    return ok;
}

void DataBaseConnectionDialog::saveConnectionDataToEnvironment()
{
    std::string dbUserEnvVariable = TestDBConnection::makeEnvironmentVariableName("DBUSERNAME");
    qputenv(dbUserEnvVariable.c_str(), m_qt_MySqlUser->text().toUtf8());
    std::cerr << "qputenv " << dbUserEnvVariable << m_qt_MySqlUser->text().toStdString() << std::endl;

    std::string dbPasswordEnvVariable = TestDBConnection::makeEnvironmentVariableName("DBPASSWORD");
    qputenv(dbPasswordEnvVariable.c_str(), m_qt_MySqlPassword->text().toUtf8());
    std::cerr << "qputenv " << dbPasswordEnvVariable << m_qt_MySqlPassword->text().toStdString() << std::endl;
}

QGroupBox *DataBaseConnectionDialog::setUpOptionsGB()
{
    optionsGB = new QGroupBox("Program Options", this);
    optionsGB->setObjectName("optionsGB");

    optionsFormLayout = cqtfa_FormLayoutWithPolicy("optionsFormLayout", optionsGB);

    verboseOutput = cqtfa_QTWidgetWithText<QCheckBox>("Enable Verbose Output",
        "verboseOutput", optionsGB);
    optionsFormLayout->addWidget(verboseOutput);

    runSelftTest = cqtfa_QTWidgetWithText<QCheckBox>("Enable Self Test",
        "runSelftTest", optionsGB);
    optionsFormLayout->addWidget(runSelftTest);

    forceErrors = cqtfa_QTWidgetWithText<QCheckBox>("Force Errors",
        "forceErrors", optionsGB);
    optionsFormLayout->addWidget(forceErrors);

    forceExceptions = cqtfa_QTWidgetWithText<QCheckBox>("Force Exceptions",
        "forceExceptions", optionsGB);
    optionsFormLayout->addWidget(forceExceptions);

    return optionsGB;
}

std::string DataBaseConnectionDialog::makeEnvironmentVariableName(std::string vName)
{
    std::string environmentVariableName("CHERNICKSW_PLANNERORGANIZER_");

    environmentVariableName.append(vName);

    return environmentVariableName;
}

void DataBaseConnectionDialog::checkEnvironmentVariables()
{
    programOptions.mySqlUser = qEnvironmentVariable(makeEnvironmentVariableName("DBUSERNAME").c_str()).toStdString();

    programOptions.mySqlPassword = qEnvironmentVariable(makeEnvironmentVariableName("DBPASSWORD").c_str()).toStdString();

    programOptions.mySqlDBName = qEnvironmentVariable(makeEnvironmentVariableName("DATABASE").c_str()).toStdString();

    programOptions.mySqlUrl = qEnvironmentVariable(makeEnvironmentVariableName("DBURL").c_str()).toStdString();

    programOptions.mySqlPort = qEnvironmentVariableIntValue(makeEnvironmentVariableName("DBPORT").c_str());
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
