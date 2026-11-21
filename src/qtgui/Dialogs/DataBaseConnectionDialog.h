#ifndef DATABASECONNECTIONDIALOG_H_
#define DATABASECONNECTIONDIALOG_H_

// Project Header Files

// QT Header Files
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
    
    const int MaxQLineEditCharCount = 40;
    const int MarginAndSpacing = 30;
    int m_maxGroupBoxHeight;
    int m_maxButtonBoxHeight;

    QVBoxLayout* m_qt_dialogLayout = nullptr;
    QGroupBox* m_qt_connectionsGB = nullptr;
    QFormLayout* m_qt_groupBoxFormLayout = nullptr;
    QLineEdit* m_qt_mySqlUser = nullptr;
    QLineEdit* m_qt_mySqlPassword = nullptr;
    QLineEdit* m_qt_mySqlUrl = nullptr;
    QLineEdit* m_qt_mySqlPort = nullptr;
    QLineEdit* m_qt_mySqlDBName = nullptr;
    QDialogButtonBox* m_qt_buttonBox = nullptr;
    QGroupBox* m_qt_optionsGB = nullptr;
    QFormLayout* m_qt_optionsFormLayout = nullptr;
    QCheckBox* m_qt_verboseOutput = nullptr;
    QCheckBox* m_qt_runSelftTest = nullptr;
    QCheckBox* m_qt_forceErrors = nullptr;
    QCheckBox* m_qt_forceExceptions = nullptr;
};

#endif // DATABASECONNECTIONDIALOG_H_
