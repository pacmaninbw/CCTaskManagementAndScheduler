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
      m_UserID{0},
      m_UserData{nullptr}
{
    m_UserData = std::make_shared<UserModel>();

    setUpUserEditorDialogUi();
}

UserEditorDialog::UserEditorDialog(std::shared_ptr<UserModel> userData, QWidget *parent)
    : QDialog(parent),
      m_UserID{userData->getUserID()},
      m_UserData{userData}
{
    setUpUserEditorDialogUi();

    initAllFieldsFromData();
}

UserEditorDialog::~UserEditorDialog()
{

}

void UserEditorDialog::accept()
{
    bool updateSuccessful = (m_UserID > 0)? upDateUser() : addUser();
    
    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "User edit failed.\n";
        errorReport += QString::fromStdString(m_UserData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void UserEditorDialog::setUpUserEditorDialogUi()
{
    m_qt_EditorLayout = cqtfa_QTWidget<QVBoxLayout>("m_qt_EditorLayout", this);
    m_qt_EditorLayout->addWidget(setUpUserProfileGB(), 0, Qt::AlignHCenter);
    m_qt_EditorLayout->addWidget(setUpLoginDataGB(), 0, Qt::AlignHCenter);

    m_qt_ButtonBox = setUpEditUserButtonBox();
    m_qt_EditorLayout->addWidget(m_qt_ButtonBox, 0, Qt::AlignHCenter);

    m_qt_EditorLayout->setSpacing(groupBoxSpacing);

    setLayout(m_qt_EditorLayout);

    QString dialogTitle = (m_UserID == 0)? "Add User" : "Edit User Profile";

    setWindowTitle(dialogTitle);
}

QGroupBox* UserEditorDialog::setUpUserProfileGB()
{
    m_qt_UserProfileGB = new QGroupBox("User Profile:");

    m_qt_UserProfileForm = cqtfa_FormLayoutWithPolicy("m_qt_UserProfileForm", m_qt_UserProfileGB);

    m_qt_FirstName = cqtfa_LineEditWithWidthAndLength("m_qt_FirstName", this);
    m_qt_UserProfileForm->addRow("FirstName:", m_qt_FirstName);

    m_qt_LastName = cqtfa_LineEditWithWidthAndLength("m_qt_LastName", this);
    m_qt_UserProfileForm->addRow("Last Name:", m_qt_LastName);

    m_qt_MiddleName = cqtfa_LineEditWithWidthAndLength("m_qt_MiddleName", this);
    m_qt_UserProfileForm->addRow("Middle Initial:", m_qt_MiddleName);

    m_qt_Email = cqtfa_LineEditWithWidthAndLength("m_qt_Email", this, emailWidth, emailCharCount);
    m_qt_UserProfileForm->addRow("eMail Address:", m_qt_Email);

    m_qt_UserProfileGB->setLayout(m_qt_UserProfileForm);

    return m_qt_UserProfileGB;
}

QGroupBox* UserEditorDialog::setUpLoginDataGB()
{
    m_qt_LoginGB = new QGroupBox("User Login:");

    m_qt_LoginForm = cqtfa_FormLayoutWithPolicy("m_qt_LoginForm", m_qt_LoginGB);

    m_qt_UserName = cqtfa_LineEditWithWidthAndLength("m_qt_UserName", this);
    m_qt_LoginForm->addRow("User Name:", m_qt_UserName);

    m_qt_Password = cqtfa_LineEditWithWidthAndLength("m_qt_Password", this);
    m_qt_LoginForm->addRow("Password:", m_qt_Password);

    if (m_UserID == 0)
    {
        m_qt_AutoGenerateLoginAndPassword = cqtfa_QTWidgetWithText<QCheckBox>(
            "Auto Generate Login and Password", "m_qt_AutoGenerateLoginAndPassword", this);
        m_qt_LoginForm->addRow(m_qt_AutoGenerateLoginAndPassword);
    }
    else
    {
        m_qt_AutoGenerateLoginAndPassword = nullptr;
    }
    
    m_qt_LoginGB->setLayout(m_qt_LoginForm);

    return m_qt_LoginGB;
}

QDialogButtonBox *UserEditorDialog::setUpEditUserButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("m_qt_ButtonBox"));
    buttonBox->setGeometry(QRect(0, 500, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void UserEditorDialog::initAllFieldsFromData()
{
    m_qt_FirstName->setText(QString::fromStdString(m_UserData->getFirstName()));
    m_qt_LastName->setText(QString::fromStdString(m_UserData->getLastName()));
    m_qt_MiddleName->setText(QString::fromStdString(m_UserData->getMiddleInitial()));
    m_qt_Email->setText(QString::fromStdString(m_UserData->getEmail()));
    m_qt_UserName->setText(QString::fromStdString(m_UserData->getLoginName()));
    m_qt_Password->setText(QString::fromStdString(m_UserData->getPassword()));
}

bool UserEditorDialog::addUser()
{
    transferAllFieldsToData();
    return m_UserData->insert();
}

bool UserEditorDialog::upDateUser()
{
    transferAllFieldsToData();
    return m_UserData->update();
}

void UserEditorDialog::transferAllFieldsToData()
{
    m_UserData->setFirstName(m_qt_FirstName->text().toStdString());
    m_UserData->setLastName(m_qt_LastName->text().toStdString());
    m_UserData->setMiddleInitial(m_qt_MiddleName->text().toStdString());
    m_UserData->setEmail(m_qt_Email->text().toStdString());
    
    if (m_UserID == 0 && m_qt_AutoGenerateLoginAndPassword->isChecked())
    {
        m_UserData->autoGenerateLoginAndPassword();
    }
    else
    {
        m_UserData->setLoginName(m_qt_UserName->text().toStdString());
        m_UserData->setPassword(m_qt_Password->text().toStdString());
    }
}
