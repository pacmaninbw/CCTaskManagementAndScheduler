// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
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
#include <memory>

UserEditorDialog::UserEditorDialog(QWidget *parent)
    : QDialog(parent),
      m_userID{0},
      m_userData{nullptr}
{
    m_userData = std::make_shared<UserModel>();

    setUpUserEditorDialogUi();
}

UserEditorDialog::UserEditorDialog(std::shared_ptr<UserModel> userData, QWidget *parent)
    : QDialog(parent),
      m_userID{userData->getUserID()},
      m_userData{userData}
{
    setUpUserEditorDialogUi();

    initAllFieldsFromData();
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
        errorReport += QString::fromStdString(m_userData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void UserEditorDialog::setUpUserEditorDialogUi()
{
    m_qt_editorLayout = cqtfa_QTWidget<QVBoxLayout>("m_qt_editorLayout", this);
    m_qt_editorLayout->addWidget(setUpUserProfileGB(), 0, Qt::AlignHCenter);
    m_qt_editorLayout->addWidget(setUpLoginDataGB(), 0, Qt::AlignHCenter);

    m_qt_buttonBox = setUpEditUserButtonBox();
    m_qt_editorLayout->addWidget(m_qt_buttonBox, 0, Qt::AlignHCenter);

    m_qt_editorLayout->setSpacing(GroupBoxSpacing);

    setLayout(m_qt_editorLayout);

    QString dialogTitle = (m_userID == 0)? "Add User" : "Edit User Profile";

    setWindowTitle(dialogTitle);
}

QGroupBox* UserEditorDialog::setUpUserProfileGB()
{
    m_qt_userProfile = new QGroupBox("User Profile:");

    m_qt_userProfileForm = cqtfa_FormLayoutWithPolicy("m_qt_userProfileForm", m_qt_userProfile);

    m_qt_firstName = cqtfa_LineEditWithWidthAndLength("m_qt_firstName", this);
    m_qt_userProfileForm->addRow("FirstName:", m_qt_firstName);

    m_qt_lastName = cqtfa_LineEditWithWidthAndLength("m_qt_lastName", this);
    m_qt_userProfileForm->addRow("Last Name:", m_qt_lastName);

    m_qt_middleName = cqtfa_LineEditWithWidthAndLength("m_qt_middleName", this);
    m_qt_userProfileForm->addRow("Middle Initial:", m_qt_middleName);

    m_qt_email = cqtfa_LineEditWithWidthAndLength("m_qt_email", this, EmailWidth, EmailCharCount);
    m_qt_userProfileForm->addRow("eMail Address:", m_qt_email);

    m_qt_userProfile->setLayout(m_qt_userProfileForm);

    return m_qt_userProfile;
}

QGroupBox* UserEditorDialog::setUpLoginDataGB()
{
    m_qt_login = new QGroupBox("User Login:");

    m_qt_loginForm = cqtfa_FormLayoutWithPolicy("m_qt_loginForm", m_qt_login);

    m_qt_userName = cqtfa_LineEditWithWidthAndLength("m_qt_userName", this);
    m_qt_loginForm->addRow("User Name:", m_qt_userName);

    m_qt_password = cqtfa_LineEditWithWidthAndLength("m_qt_password", this);
    m_qt_loginForm->addRow("Password:", m_qt_password);

    if (m_userID == 0)
    {
        m_qt_autoGenerateLoginAndPassword = cqtfa_QTWidgetWithText<QCheckBox>(
            "Auto Generate Login and Password", "m_qt_autoGenerateLoginAndPassword", this);
        m_qt_loginForm->addRow(m_qt_autoGenerateLoginAndPassword);
    }
    else
    {
        m_qt_autoGenerateLoginAndPassword = nullptr;
    }
    
    m_qt_login->setLayout(m_qt_loginForm);

    return m_qt_login;
}

QDialogButtonBox *UserEditorDialog::setUpEditUserButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("m_qt_buttonBox"));
    buttonBox->setGeometry(QRect(0, 500, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void UserEditorDialog::initAllFieldsFromData()
{
    m_qt_firstName->setText(QString::fromStdString(m_userData->getFirstName()));
    m_qt_lastName->setText(QString::fromStdString(m_userData->getLastName()));
    m_qt_middleName->setText(QString::fromStdString(m_userData->getMiddleInitial()));
    m_qt_email->setText(QString::fromStdString(m_userData->getEmail()));
    m_qt_userName->setText(QString::fromStdString(m_userData->getLoginName()));
    m_qt_password->setText(QString::fromStdString(m_userData->getPassword()));
}

bool UserEditorDialog::addUser()
{
    transferAllFieldsToData();
    return m_userData->insert();
}

bool UserEditorDialog::upDateUser()
{
    transferAllFieldsToData();
    return m_userData->update();
}

void UserEditorDialog::transferAllFieldsToData()
{
    m_userData->setFirstName(m_qt_firstName->text().toStdString());
    m_userData->setLastName(m_qt_lastName->text().toStdString());
    m_userData->setMiddleInitial(m_qt_middleName->text().toStdString());
    m_userData->setEmail(m_qt_email->text().toStdString());
    
    if (m_userID == 0 && m_qt_autoGenerateLoginAndPassword->isChecked())
    {
        m_userData->autoGenerateLoginAndPassword();
    }
    else
    {
        m_userData->setLoginName(m_qt_userName->text().toStdString());
        m_userData->setPassword(m_qt_password->text().toStdString());
    }
}
