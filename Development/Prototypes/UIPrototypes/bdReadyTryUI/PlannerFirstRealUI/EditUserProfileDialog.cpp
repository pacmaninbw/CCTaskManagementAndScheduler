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
#include <format>
#include <string>
#include <iostream>


EditUserProfileDialog::EditUserProfileDialog(QWidget *parent)
    : QDialog(parent),
    userID{0}
{
    setUpEditUserProfileDialogUi();
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

    firstNameLE = createNamedLineEditWithWidthAndLength("firstNameLE");
    userProfileForm->addRow("FirstName:", firstNameLE);

    lastNameLE = createNamedLineEditWithWidthAndLength("lastNameLE");
    userProfileForm->addRow("Last Name:", lastNameLE);

    middleNameLE = createNamedLineEditWithWidthAndLength("middleNameLE");
    userProfileForm->addRow("Middle Initial:", middleNameLE);

    emailLE = createNamedLineEditWithWidthAndLength("emailLE");
    userProfileForm->addRow("eMail Address:", emailLE);

    userProfileGB->setLayout(userProfileForm);

    return userProfileGB;
}

QGroupBox *EditUserProfileDialog::setUpLoginDataGB()
{
    std::cout << "In setUpLoginDataGB" << std::endl;

    loginDataGB = new QGroupBox("User Login:");

    loginDataForm = createNamedFormLayoutWithPolicy("loginDataForm");

    userNameLE = createNamedLineEditWithWidthAndLength("userNameLE");
    loginDataForm->addRow("User Name:", userNameLE);

    passwordLE = createNamedLineEditWithWidthAndLength("passwordLE");
    loginDataForm->addRow("Password:", passwordLE);
    
    loginDataGB->setLayout(loginDataForm);

    return loginDataGB;
}

QDialogButtonBox *EditUserProfileDialog::setUpEditUserButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("editUserButtonBox"));
    buttonBox->setGeometry(QRect(0, 500, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    return buttonBox;
}

QFormLayout *EditUserProfileDialog::createNamedFormLayoutWithPolicy(const char *formName)
{
    QFormLayout* newFormLayout = createNamedQTWidget<QFormLayout>(formName);
    newFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    return newFormLayout;
}

QLineEdit *EditUserProfileDialog::createNamedLineEditWithWidthAndLength(const char *lineEditName, std::size_t width, std::size_t charCount)
{
    QLineEdit* newLineEdit = createNamedQTWidget<QLineEdit>(lineEditName, this);
    std::string styleSheetStr(std::format("width: {}px;", width));
    newLineEdit->setStyleSheet(styleSheetStr.c_str());
    newLineEdit->setMaxLength(charCount);

    return newLineEdit;
}
