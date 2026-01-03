// Project Header Files
#include "createNamedQTWidget.h"
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

    QFormLayout* userLoginFormLayout = createNamedFormLayoutWithPolicy("userLogingFormLayout", userLoginGB);

    userLoginFormLayout->addRow("User Name:", createNamedLineEditWithWidthAndLength("userLoginUserNameLE"));

    userLoginFormLayout->addRow("Password:", createNamedLineEditWithWidthAndLength("userLoginPasswordLE"));

    actionLoginAsUserPB = createNameQTWidgetWithText<QPushButton>("Login As User", "actionLoginAsUserPB", userLoginGB);
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
