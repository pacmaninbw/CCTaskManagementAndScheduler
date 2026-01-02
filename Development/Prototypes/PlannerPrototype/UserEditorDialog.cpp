// Project Header Files
#include "createNamedQTWidget.h"
#include "UserEditorDialog.h"

// QT Header Files
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



// Standard C++ Header Files

UserEditorDialog::UserEditorDialog(QWidget *parent)
    : QDialog(parent),
    userID{0}
{
    setUpUserEditorialogUi();
}

UserEditorDialog::~UserEditorDialog()
{

}
void UserEditorDialog::setUpUserEditorialogUi()
{
    editUserLayout = createNamedQTWidget<QVBoxLayout>("editUserLayout", this);
    editUserLayout->addWidget(setUpUserProfileGB(), 0, Qt::AlignHCenter);
    editUserLayout->addWidget(setUpLoginDataGB(), 0, Qt::AlignHCenter);

    editUserButtonBox = setUpEditUserButtonBox();
    editUserLayout->addWidget(editUserButtonBox, 0, Qt::AlignHCenter);

    editUserLayout->setSpacing(groupBoxSpacing);

    setLayout(editUserLayout);

    QString dialogTitle = (userID == 0)? "Add User" : "Edit User Profile";

    setWindowTitle(dialogTitle);

}

QGroupBox* UserEditorDialog::setUpUserProfileGB()
{
    userProfileGB = new QGroupBox("User Profile:");

    userProfileForm = createNamedFormLayoutWithPolicy("userProfileForm", userProfileGB);

    firstNameLE = createNamedLineEditWithWidthAndLength("firstNameLE", this);
    userProfileForm->addRow("FirstName:", firstNameLE);

    lastNameLE = createNamedLineEditWithWidthAndLength("lastNameLE", this);
    userProfileForm->addRow("Last Name:", lastNameLE);

    middleNameLE = createNamedLineEditWithWidthAndLength("middleNameLE", this);
    userProfileForm->addRow("Middle Initial:", middleNameLE);

    emailLE = createNamedLineEditWithWidthAndLength("emailLE", this);
    userProfileForm->addRow("eMail Address:", emailLE);

    userProfileGB->setLayout(userProfileForm);

    return userProfileGB;
}

QGroupBox* UserEditorDialog::setUpLoginDataGB()
{
    loginDataGB = new QGroupBox("User Login:");

    loginDataForm = createNamedFormLayoutWithPolicy("loginDataForm", loginDataGB);

    userNameLE = createNamedLineEditWithWidthAndLength("userNameLE", this);
    loginDataForm->addRow("User Name:", userNameLE);

    passwordLE = createNamedLineEditWithWidthAndLength("passwordLE", this);
    loginDataForm->addRow("Password:", passwordLE);
    
    loginDataGB->setLayout(loginDataForm);

    return loginDataGB;
}

QDialogButtonBox *UserEditorDialog::setUpEditUserButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("editUserButtonBox"));
    buttonBox->setGeometry(QRect(0, 500, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}



