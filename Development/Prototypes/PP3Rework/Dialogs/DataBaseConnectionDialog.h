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

    QVBoxLayout* dbConnectionsDialogLayout;
    QGroupBox* dbConnectionsGB;
    QFormLayout* dbConnectionsFormLayout;
    QLineEdit* mySqlUser;
    QLineEdit* mySqlPassword;
    QLineEdit* mySqlUrl;
    QLineEdit* mySqlPort;
    QLineEdit* mySqlDBName;
    QCheckBox* rememberMe;
    QDialogButtonBox* dbConnectionsButtonBox;
    QGroupBox* optionsGB;
    QFormLayout* optionsFormLayout;
    QCheckBox* verboseOutput;
    QCheckBox* runSelftTest;
    QCheckBox* forceErrors;
    QCheckBox* forceExceptions;
};

#endif // DATABASECONNECTIONDIALOG_H
