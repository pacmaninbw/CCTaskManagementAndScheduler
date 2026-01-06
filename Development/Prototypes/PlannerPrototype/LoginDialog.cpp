// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "LoginDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QVBoxLayout>

// Standard C++ Header Files

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
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

    userLoginFormLayout->addRow("User Name:", cqtfa_LineEditWithWidthAndLength("userLoginUserNameLE", userLoginGB));

    userLoginFormLayout->addRow("Password:", cqtfa_LineEditWithWidthAndLength("userLoginPasswordLE", userLoginGB));

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
    accept();
}
