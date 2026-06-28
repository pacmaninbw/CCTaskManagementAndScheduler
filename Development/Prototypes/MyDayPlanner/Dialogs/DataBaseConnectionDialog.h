#ifndef DATABASECONNECTIONDIALOG_H
#define DATABASECONNECTIONDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>

// Standard C++ Header Files

class DataBaseConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataBaseConnectionDialog(QWidget *parent = nullptr);
    ~DataBaseConnectionDialog();

Q_SIGNALS:

private Q_SLOTS:
    void on_dbConnectionsButtonBox_accepted();

private:
    void setUpDBConnectionDialogUI();
    QFormLayout* setUpDBConnectionsFormLayout();
    QDialogButtonBox* setUpButtonBox();
    void limitDialogRowth();
    bool testConnection();
    bool validateAndUpdateProgramOption();
    void saveConnectionDataToEnvironment();
    QGroupBox* setUpOptionsGB();
    std::string makeEnvironmentVariableName(std::string vName);
    void checkEnvironmentVariables();
    
    const int maxQLineEditCharCount = 40;
    const int marginAndSpacing = 30;
    int maxGroupBoxHeight;
    int maxButtonBoxHeight;

    QVBoxLayout* dbConnectionsDialogLayout = nullptr;
    QGroupBox* dbConnectionsGB = nullptr;
    QFormLayout* dbConnectionsFormLayout = nullptr;
    QLineEdit* mySqlUser = nullptr;
    QLineEdit* mySqlPassword = nullptr;
    QLineEdit* mySqlUrl = nullptr;
    QLineEdit* mySqlPort = nullptr;
    QLineEdit* mySqlDBName = nullptr;
    QCheckBox* rememberMe = nullptr;
    QDialogButtonBox* dbConnectionsButtonBox = nullptr;
    QGroupBox* optionsGB = nullptr;
    QFormLayout* optionsFormLayout = nullptr;
    QCheckBox* verboseOutput = nullptr;
    QCheckBox* runSelftTest = nullptr;
    QCheckBox* forceErrors = nullptr;
    QCheckBox* forceExceptions = nullptr;
};

#endif // DATABASECONNECTIONDIALOG_H
