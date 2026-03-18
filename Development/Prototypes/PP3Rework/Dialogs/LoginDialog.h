#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

class UserModel;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QApplication>
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
    std::shared_ptr<UserModel> GetUserData() { return m_UserDataPtr; };

private Q_SLOTS:
    void onactionLoginAsUserPBClicked();

private:
    void setUpLoginDialogUI();
    bool attemptLogin();

    std::shared_ptr<UserModel> m_UserDataPtr;

    QGroupBox* userLoginGB;
    QLineEdit* userLoginUserNameLE;
    QLineEdit* userLoginPasswordLE;
    QPushButton* actionLoginAsUserPB;

    const int userLoginDialogWidth = 400;
    const int userLoginDialogHeight = 200;

};

#endif // LOGINDIALOG_H
