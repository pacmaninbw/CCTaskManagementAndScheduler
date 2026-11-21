#ifndef LOGINDIALOG_H_
#define LOGINDIALOG_H_

class UserModel;

// Project Header Files

// QT Header Files
#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

// Standard C++ Header Files
#include <memory>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();
    std::shared_ptr<UserModel> GetUserData() { return m_userDataPtr; };

private Q_SLOTS:
    void onactionLoginAsUserPBClicked();

private:
    const int UserLoginDialogWidth = 400;
    const int UserLoginDialogHeight = 200;

    void setUpLoginDialogUI();

    std::shared_ptr<UserModel> m_userDataPtr;

    QGroupBox* m_qt_userLoginGB = nullptr;
    QLineEdit* m_qt_userName = nullptr;
    QLineEdit* m_qt_password = nullptr;
    QPushButton* m_qt_loginAsUser = nullptr;
};

#endif // LOGINDIALOG_H_
