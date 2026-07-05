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
    m_maxGroupBoxHeight{0},
    m_maxButtonBoxHeight{0}
{
    checkEnvironmentVariables();

    setUpDBConnectionDialogUI();
}

DataBaseConnectionDialog::~DataBaseConnectionDialog()
{

}

void DataBaseConnectionDialog::setUpDBConnectionDialogUI()
{
    m_qt_dialogLayout = new QVBoxLayout(this);
    m_qt_dialogLayout->setObjectName("m_qt_dialogLayout");

    m_qt_connectionsGB = new QGroupBox("MySQL Database Connection Data", this);
    m_qt_connectionsGB->setObjectName("mysqlConnectionsGB");

    m_qt_connectionsGB->setLayout(setUpDBConnectionsFormLayout());

    m_qt_dialogLayout->addWidget(m_qt_connectionsGB);

    m_qt_dialogLayout->addWidget(setUpOptionsGB());
    
    m_qt_dialogLayout->addWidget(setUpButtonBox(), 0 , Qt::AlignHCenter);

    setLayout(m_qt_dialogLayout);

    limitDialogRowth();

    adjustSize();

    setWindowTitle("Database Connection Dialog");
}

QFormLayout *DataBaseConnectionDialog::setUpDBConnectionsFormLayout()
{
    m_qt_groupBoxFormLayout = cqtfa_FormLayoutWithPolicy("m_qt_groupBoxFormLayout", m_qt_connectionsGB);

    m_qt_mySqlUser = cqtfa_LineEditFixedWidthByCharCount("m_qt_mySqlUser", m_qt_connectionsGB, MaxQLineEditCharCount);
    m_qt_mySqlUser->setText(QString::fromStdString(programOptions.mySqlUser));
    m_qt_groupBoxFormLayout->addRow("MySQL User Name:", m_qt_mySqlUser);

    m_qt_mySqlPassword = cqtfa_LineEditFixedWidthByCharCount("m_qt_mySqlPassword", m_qt_connectionsGB, MaxQLineEditCharCount);
    m_qt_mySqlPassword->setText(QString::fromStdString(programOptions.mySqlPassword));
    m_qt_groupBoxFormLayout->addRow("MySQL User Password:", m_qt_mySqlPassword);

    m_qt_mySqlUrl = cqtfa_LineEditFixedWidthByCharCount("m_qt_mySqlUrl", m_qt_connectionsGB, MaxQLineEditCharCount);
    m_qt_mySqlUrl->setText(QString::fromStdString(programOptions.mySqlUrl));
    m_qt_groupBoxFormLayout->addRow("MySQL URL:", m_qt_mySqlUrl);

    m_qt_mySqlPort = cqtfa_LineEditFixedWidthByCharCount("m_qt_mySqlPort", m_qt_connectionsGB, MaxQLineEditCharCount);
    m_qt_mySqlPort->setText(QString::number(programOptions.mySqlPort));
    m_qt_groupBoxFormLayout->addRow("MySQL Port:", m_qt_mySqlPort);

    m_qt_mySqlDBName = cqtfa_LineEditFixedWidthByCharCount("m_qt_mySqlDBName", m_qt_connectionsGB, MaxQLineEditCharCount);
    m_qt_mySqlDBName->setText(QString::fromStdString(programOptions.mySqlDBName));
    m_qt_groupBoxFormLayout->addRow("MySQL Database Name:", m_qt_mySqlDBName);

    m_maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_qt_groupBoxFormLayout);


    return m_qt_groupBoxFormLayout;
}

QDialogButtonBox *DataBaseConnectionDialog::setUpButtonBox()
{
    m_qt_buttonBox = new QDialogButtonBox(this);

    m_qt_buttonBox->setObjectName(QString::fromUtf8("editNoteButtonBox"));
    m_qt_buttonBox->setOrientation(Qt::Horizontal);
    m_qt_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    m_maxButtonBoxHeight = m_qt_buttonBox->height() + MarginAndSpacing;

    connect(m_qt_buttonBox, &QDialogButtonBox::accepted, this, &DataBaseConnectionDialog::on_dbConnectionsButtonBox_accepted);
    connect(m_qt_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return m_qt_buttonBox;
}

void DataBaseConnectionDialog::limitDialogRowth()
{
    m_qt_connectionsGB->setMaximumHeight(m_maxGroupBoxHeight);
    m_qt_connectionsGB->setMaximumWidth(cqtfa_getFormLayoutMaxWidth(m_qt_groupBoxFormLayout) + MarginAndSpacing);

    m_qt_buttonBox->setMaximumHeight(m_maxButtonBoxHeight);

    int maxDialogWidth = m_qt_connectionsGB->maximumWidth() + MarginAndSpacing;

    setMaximumWidth(maxDialogWidth);

    setMaximumHeight(m_maxGroupBoxHeight + m_maxButtonBoxHeight + MarginAndSpacing);
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

    std::string inputUserName = m_qt_mySqlUser->text().toStdString();
    std::string inputPassword = m_qt_mySqlPassword->text().toStdString();

    if (inputUserName.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Name\n");
    }

    if (inputPassword.empty())
    {
        errorMessages.append("Missing Required field: MySQL User Password\n");
    }

    programOptions.mySqlPort = m_qt_mySqlPort->text().toUInt(&ok);
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
    programOptions.mySqlUrl = m_qt_mySqlUrl->text().toStdString();
    programOptions.mySqlDBName = m_qt_mySqlDBName->text().toStdString();
    programOptions.verboseOutput = m_qt_verboseOutput->isChecked();
    programOptions.runSelfTest = m_qt_runSelftTest->isChecked();
    programOptions.forceErrors = m_qt_forceErrors->isChecked();
    programOptions.forceExceptions = m_qt_forceExceptions->isChecked();

    return ok;
}

void DataBaseConnectionDialog::saveConnectionDataToEnvironment()
{
    std::string dbUserEnvVariable = TestDBConnection::makeEnvironmentVariableName("DBUSERNAME");
    qputenv(dbUserEnvVariable.c_str(), m_qt_mySqlUser->text().toUtf8());
    std::cerr << "qputenv " << dbUserEnvVariable << m_qt_mySqlUser->text().toStdString() << std::endl;

    std::string dbPasswordEnvVariable = TestDBConnection::makeEnvironmentVariableName("DBPASSWORD");
    qputenv(dbPasswordEnvVariable.c_str(), m_qt_mySqlPassword->text().toUtf8());
    std::cerr << "qputenv " << dbPasswordEnvVariable << m_qt_mySqlPassword->text().toStdString() << std::endl;
}

QGroupBox *DataBaseConnectionDialog::setUpOptionsGB()
{
    m_qt_optionsGB = new QGroupBox("Program Options", this);
    m_qt_optionsGB->setObjectName("m_qt_optionsGB");

    m_qt_optionsFormLayout = cqtfa_FormLayoutWithPolicy("m_qt_optionsFormLayout", m_qt_optionsGB);

    m_qt_verboseOutput = cqtfa_QTWidgetWithText<QCheckBox>("Enable Verbose Output",
        "verboseOutput", m_qt_optionsGB);
    m_qt_optionsFormLayout->addWidget(m_qt_verboseOutput);

    m_qt_runSelftTest = cqtfa_QTWidgetWithText<QCheckBox>("Enable Self Test",
        "m_qt_runSelftTest", m_qt_optionsGB);
    m_qt_optionsFormLayout->addWidget(m_qt_runSelftTest);

    m_qt_forceErrors = cqtfa_QTWidgetWithText<QCheckBox>("Force Errors",
        "forceErrors", m_qt_optionsGB);
    m_qt_optionsFormLayout->addWidget(m_qt_forceErrors);

    m_qt_forceExceptions = cqtfa_QTWidgetWithText<QCheckBox>("Force Exceptions",
        "m_qt_forceExceptions", m_qt_optionsGB);
    m_qt_optionsFormLayout->addWidget(m_qt_forceExceptions);

    return m_qt_optionsGB;
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
