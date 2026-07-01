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

    m_qt_UserLoginGB = new QGroupBox("Login As User", this);
    m_qt_UserLoginGB->setObjectName("m_qt_UserLoginGB");

    QFormLayout* userLoginFormLayout = cqtfa_FormLayoutWithPolicy("userLogingFormLayout", m_qt_UserLoginGB);

    m_qt_UserName = cqtfa_LineEditWithWidthAndLength("m_qt_UserName", m_qt_UserLoginGB);
    userLoginFormLayout->addRow("User Name:", m_qt_UserName);

    m_qt_Password = cqtfa_LineEditWithWidthAndLength("m_qt_Password", m_qt_UserLoginGB);
    userLoginFormLayout->addRow("Password:", m_qt_Password);

    m_qt_LoginAsUser = cqtfa_QTWidgetWithText<QPushButton>("Login As User", "m_qt_LoginAsUser", m_qt_UserLoginGB);
    connect(m_qt_LoginAsUser, &QPushButton::clicked, this, &LoginDialog::onactionLoginAsUserPBClicked);
    userLoginFormLayout->addWidget(m_qt_LoginAsUser);

    QVBoxLayout* userLoginDialogLayOut = new QVBoxLayout(this);
    userLoginDialogLayOut->setObjectName("userLoginDialogLayOut");

    userLoginDialogLayOut->addStretch();
    userLoginDialogLayOut->addWidget(m_qt_UserLoginGB, 0, Qt::AlignCenter);
    userLoginDialogLayOut->addStretch();

    setLayout(userLoginDialogLayOut);
    setWindowTitle("User Login");
}

void LoginDialog::onactionLoginAsUserPBClicked()
{
    QString loginName = m_qt_UserName->text();
    if (loginName.length() < 1)
    {
        QMessageBox::critical(nullptr, "Critical Error", "Missing Login Name", QMessageBox::Ok);
    }

    QString password(m_qt_Password->text());
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
