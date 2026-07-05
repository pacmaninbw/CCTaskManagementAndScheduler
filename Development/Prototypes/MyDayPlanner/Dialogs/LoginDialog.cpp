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
    m_userDataPtr{nullptr}
{
    setUpLoginDialogUI();
}

LoginDialog::~LoginDialog()
{

}

void LoginDialog::setUpLoginDialogUI()
{
    resize(UserLoginDialogWidth, UserLoginDialogHeight);

    m_qt_userLoginGB = new QGroupBox("Login As User", this);
    m_qt_userLoginGB->setObjectName("m_qt_userLoginGB");

    QFormLayout* userLoginFormLayout = cqtfa_FormLayoutWithPolicy("userLogingFormLayout", m_qt_userLoginGB);

    m_qt_userName = cqtfa_LineEditWithWidthAndLength("m_qt_userName", m_qt_userLoginGB);
    userLoginFormLayout->addRow("User Name:", m_qt_userName);

    m_qt_password = cqtfa_LineEditWithWidthAndLength("m_qt_password", m_qt_userLoginGB);
    userLoginFormLayout->addRow("Password:", m_qt_password);

    m_qt_loginAsUser = cqtfa_QTWidgetWithText<QPushButton>("Login As User", "m_qt_loginAsUser", m_qt_userLoginGB);
    connect(m_qt_loginAsUser, &QPushButton::clicked, this, &LoginDialog::onactionLoginAsUserPBClicked);
    userLoginFormLayout->addWidget(m_qt_loginAsUser);

    QVBoxLayout* userLoginDialogLayOut = new QVBoxLayout(this);
    userLoginDialogLayOut->setObjectName("userLoginDialogLayOut");

    userLoginDialogLayOut->addStretch();
    userLoginDialogLayOut->addWidget(m_qt_userLoginGB, 0, Qt::AlignCenter);
    userLoginDialogLayOut->addStretch();

    setLayout(userLoginDialogLayOut);
    setWindowTitle("User Login");
}

void LoginDialog::onactionLoginAsUserPBClicked()
{
    QString loginName = m_qt_userName->text();
    if (loginName.length() < 1)
    {
        QMessageBox::critical(nullptr, "Critical Error", "Missing Login Name", QMessageBox::Ok);
    }

    QString password(m_qt_password->text());
    if (password.length() < 1)
    {
        QMessageBox::critical(nullptr, "Critical Error", "Missing Password", QMessageBox::Ok);
    }

    UserQueryProcessor userQueryProcessor;
    m_userDataPtr = userQueryProcessor.getUserByLoginAndPassword(loginName.toStdString(), password.toStdString());
    if (m_userDataPtr)
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
