#ifndef DATABASECONNECTIONDIALOG_H
#define DATABASECONNECTIONDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
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

public slots:

signals:

private slots:
    void on_dbConnectionsButtonBox_accepted();

private:
    void setUpDBConnectionDialogUI();
    QFormLayout* setUpDBConnectionsFormLayout();
    QDialogButtonBox* setUpButtonBox();
    void limitDialogRowth();
    bool testConnection();
    bool validateAndUpdateProgramOption();
    
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
    QDialogButtonBox* dbConnectionsButtonBox;
};

#endif // DATABASECONNECTIONDIALOG_H
