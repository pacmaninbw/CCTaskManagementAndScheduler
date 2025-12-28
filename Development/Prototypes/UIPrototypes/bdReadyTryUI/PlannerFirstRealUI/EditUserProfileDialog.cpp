// Project Headers
#include "EditUserProfileDialog.h"
//#include "UserModel.h"

// QT Headers
#include <QVariant>
#include <QDialog>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

// Standard C++ Headers


EditUserProfileDialog::EditUserProfileDialog(QWidget *parent)
    : QDialog(parent),
    userID{0}
{
}

EditUserProfileDialog::~EditUserProfileDialog()
{
}

void EditUserProfileDialog::setUpEditUserProfileDialogUi()
{
    editUserLayout = createNamedQTWidget<QVBoxLayout>("editUserLayout");
    editUserLayout->addWidget(setUpUserProfileGB(), 0, Qt::AlignHCenter);
    editUserLayout->addWidget(setUpLoginDataGB(), 0, Qt::AlignHCenter);

    editUserButtonBox = setUpEditUserButtonBox();
    editUserLayout->addWidget(editUserButtonBox, 0, Qt::AlignHCenter);

    editUserLayout->setSpacing(groupBoxSpacing);

    setLayout(editUserLayout);

    QString dialogTitle = (userID == 0)? "Add User" : "Edit User Profile";

    setWindowTitle(dialogTitle);

}

QGroupBox *EditUserProfileDialog::setUpUserProfileGB()
{
    userProfileGB = new QGroupBox("User Profile:");

    userProfileForm = createNamedFormLayoutWithPolicy("userProfileForm");

    firstNameLE = createNamedQTWidget<QLineEdit>("firstNameLE");
    userProfileForm->addRow("FirstName:", firstNameLE);

    lastNameLE = createNamedQTWidget<QLineEdit>("lastNameLE");
    userProfileForm->addRow("Last Name:", lastNameLE);

    middleNameLE = createNamedQTWidget<QLineEdit>("middleNameLE");
    userProfileForm->addRow("Middle Initial:", middleNameLE);

    emailLE = createNamedQTWidget<QLineEdit>("emailLE");
    userProfileForm->addRow("eMail Address:", emailLE);

    userProfileGB->setLayout(userProfileForm);

    return userProfileGB;
}

QGroupBox *EditUserProfileDialog::setUpLoginDataGB()
{
    loginDataGB = new QGroupBox("User Login:");

    loginDataForm = createNamedFormLayoutWithPolicy("loginDataForm");

    userNameLE = createNamedQTWidget<QLineEdit>("");
    loginDataForm->addRow("User Name:", userNameLE);

    passwordLE = createNamedQTWidget<QLineEdit>("passwordLE");
    loginDataForm->addRow("Password:", passwordLE);
    
    loginDataGB->setLayout(loginDataForm);

    return loginDataGB;
}

QDialogButtonBox *EditUserProfileDialog::setUpEditUserButtonBox()
{
    return nullptr;
}

QFormLayout *EditUserProfileDialog::createNamedFormLayoutWithPolicy(const char *formName)
{
    QFormLayout* newFormLayout = createNamedQTWidget<QFormLayout>(formName);
    newFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    return newFormLayout;
}
