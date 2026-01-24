// Project Header Files
#include "commonQTWidgetsForApp.h"
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
#include <QMessageBox>
#include <QVBoxLayout>


// Standard C++ Header Files

UserEditorDialog::UserEditorDialog(QWidget *parent, GuiUserModel* userData)
    : QDialog(parent),
    m_userID{0},
    m_UserData{nullptr}
{
    m_UserData = (userData != nullptr)? userData : new GuiUserModel;

    m_userID = m_UserData->getDbUserId();

    setUpUserEditorDialogUi();

    if (userData != nullptr)
    {
        initAllFieldsFromData();
    }
}

UserEditorDialog::~UserEditorDialog()
{

}

void UserEditorDialog::accept()
{
    bool updateSuccessful = (m_userID > 0)? upDateUser() : addUser();
    
    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "User edit failed.\n";
        errorReport += m_UserData->getErrorMessages();
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void UserEditorDialog::setUpUserEditorDialogUi()
{
    editUserLayout = cqtfa_QTWidget<QVBoxLayout>("editUserLayout", this);
    editUserLayout->addWidget(setUpUserProfileGB(), 0, Qt::AlignHCenter);
    editUserLayout->addWidget(setUpLoginDataGB(), 0, Qt::AlignHCenter);

    editUserButtonBox = setUpEditUserButtonBox();
    editUserLayout->addWidget(editUserButtonBox, 0, Qt::AlignHCenter);

    editUserLayout->setSpacing(groupBoxSpacing);

    setLayout(editUserLayout);

    QString dialogTitle = (m_userID == 0)? "Add User" : "Edit User Profile";

    setWindowTitle(dialogTitle);
}

QGroupBox* UserEditorDialog::setUpUserProfileGB()
{
    userProfileGB = new QGroupBox("User Profile:");

    userProfileForm = cqtfa_FormLayoutWithPolicy("userProfileForm", userProfileGB);

    firstNameLE = cqtfa_LineEditWithWidthAndLength("firstNameLE", this);
    userProfileForm->addRow("FirstName:", firstNameLE);

    lastNameLE = cqtfa_LineEditWithWidthAndLength("lastNameLE", this);
    userProfileForm->addRow("Last Name:", lastNameLE);

    middleNameLE = cqtfa_LineEditWithWidthAndLength("middleNameLE", this);
    userProfileForm->addRow("Middle Initial:", middleNameLE);

    emailLE = cqtfa_LineEditWithWidthAndLength("emailLE", this, emailWidth, emailCharCount);
    userProfileForm->addRow("eMail Address:", emailLE);

    userProfileGB->setLayout(userProfileForm);

    return userProfileGB;
}

QGroupBox* UserEditorDialog::setUpLoginDataGB()
{
    loginDataGB = new QGroupBox("User Login:");

    loginDataForm = cqtfa_FormLayoutWithPolicy("loginDataForm", loginDataGB);

    userNameLE = cqtfa_LineEditWithWidthAndLength("userNameLE", this);
    loginDataForm->addRow("User Name:", userNameLE);

    passwordLE = cqtfa_LineEditWithWidthAndLength("passwordLE", this);
    loginDataForm->addRow("Password:", passwordLE);

    if (m_userID == 0)
    {
        autoGenerateLoginAndPassword = cqtfa_QTWidgetWithText<QCheckBox>(
            "Auto Generate Login and Password", "autoGenerateLoginAndPassword", this);
        loginDataForm->addRow(autoGenerateLoginAndPassword);
    }
    else
    {
        autoGenerateLoginAndPassword = nullptr;
    }
    
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

void UserEditorDialog::initAllFieldsFromData()
{
    firstNameLE->setText(m_UserData->getFirstName());
    lastNameLE->setText(m_UserData->getLastName());
    middleNameLE->setText(m_UserData->getMiddleInitial());
    emailLE->setText(m_UserData->getEmail());
    userNameLE->setText(m_UserData->getLoginName());
    passwordLE->setText(m_UserData->getPassword());
}

bool UserEditorDialog::addUser()
{
    transferAllFieldsToData();
    return m_UserData->attemptAddUser();
}

bool UserEditorDialog::upDateUser()
{
    transferAllFieldsToData();
    return m_UserData->attemptUpdateUser();
}

void UserEditorDialog::transferAllFieldsToData()
{
    m_UserData->setFirstName(firstNameLE->text());
    m_UserData->setLastName(lastNameLE->text());
    m_UserData->setMiddleInitial(middleNameLE->text());
    m_UserData->setEmail(emailLE->text());
    m_UserData->setLoginName(userNameLE->text());
    m_UserData->setPassword(passwordLE->text());
    if (autoGenerateLoginAndPassword != nullptr)
    {
        m_UserData->setAutoGenerateLoginData(autoGenerateLoginAndPassword->isChecked());
    }
}
