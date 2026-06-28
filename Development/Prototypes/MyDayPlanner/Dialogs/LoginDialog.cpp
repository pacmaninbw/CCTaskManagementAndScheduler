// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"
#include "LoginDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>

// Standard C++ Header Files
//#include <iostream>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
    m_UserDataPtr{nullptr}
{
    setUpLoginDialogUI();
}

LoginDialog::~LoginDialog()
{

}

void LoginDialog::setUpLoginDialogUI()
{
    resize(userLoginDialogWidth, userLoginDialogHeight);

    userLoginGB = new QGroupBox("Login As User", this);
    userLoginGB->setObjectName("userLoginGB");

    QFormLayout* userLoginFormLayout = cqtfa_FormLayoutWithPolicy("userLogingFormLayout", userLoginGB);

    userLoginUserNameLE = cqtfa_LineEditWithWidthAndLength("userLoginUserNameLE", userLoginGB);
    userLoginFormLayout->addRow("User Name:", userLoginUserNameLE);

    userLoginPasswordLE = cqtfa_LineEditWithWidthAndLength("userLoginPasswordLE", userLoginGB);
    userLoginFormLayout->addRow("Password:", userLoginPasswordLE);

    actionLoginAsUserPB = cqtfa_QTWidgetWithText<QPushButton>("Login As User", "actionLoginAsUserPB", userLoginGB);
    connect(actionLoginAsUserPB, &QPushButton::clicked, this, &LoginDialog::onactionLoginAsUserPBClicked);
    userLoginFormLayout->addWidget(actionLoginAsUserPB);

    QVBoxLayout* userLoginDialogLayOut = new QVBoxLayout(this);
    userLoginDialogLayOut->setObjectName("userLoginDialogLayOut");

    userLoginDialogLayOut->addStretch();
    userLoginDialogLayOut->addWidget(userLoginGB, 0, Qt::AlignCenter);
    userLoginDialogLayOut->addStretch();

    setLayout(userLoginDialogLayOut);
    setWindowTitle("User Login");
}

void LoginDialog::onactionLoginAsUserPBClicked()
{
    QString loginName = userLoginUserNameLE->text();
    if (loginName.length() < 1)
    {
        QMessageBox::critical(nullptr, "Critical Error", "Missing Login Name", QMessageBox::Ok);
    }

    QString password(userLoginPasswordLE->text());
    if (password.length() < 1)
    {
        QMessageBox::critical(nullptr, "Critical Error", "Missing Password", QMessageBox::Ok);
    }

    UserQueryProcessor userQueryProcessor;
    m_UserDataPtr = userQueryProcessor.getUserByLoginAndPassword(loginName.toStdString(), password.toStdString());
    if (m_UserDataPtr)
    {
        accept();
    }
    else
    {
        QString errorReport = "User login failed.\n";
        errorReport += QString::fromStdString(userQueryProcessor.getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}
