#ifndef USEREDITORDIALOG_H_
#define USEREDITORDIALOG_H_

class UserModel;

// Project Header Files

// QT Header Files
#include "UserModel.h"
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <memory>

class UserEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditorDialog(QWidget *parent = nullptr);
    explicit UserEditorDialog(std::shared_ptr<UserModel> userData, QWidget *parent = nullptr);
    ~UserEditorDialog();

public Q_SLOTS:
    void accept() override;

private:
    const int GroupBoxSpacing = 60;
    const int EmailWidth = 300;
    const int EmailCharCount = 50;

    void setUpUserEditorDialogUi();
    QGroupBox* setUpUserProfileGB();
    QGroupBox* setUpLoginDataGB();
    QDialogButtonBox* setUpEditUserButtonBox();
    void initAllFieldsFromData();
    bool addUser();
    bool upDateUser();
    void transferAllFieldsToData();

    std::size_t m_userID;
    std::shared_ptr<UserModel> m_userData;

    QDialogButtonBox* m_qt_buttonBox = nullptr;
    QVBoxLayout* m_qt_editorLayout = nullptr;
    QGroupBox* m_qt_userProfile = nullptr;
    QFormLayout* m_qt_userProfileForm = nullptr;
    QLineEdit* m_qt_firstName = nullptr;
    QLineEdit* m_qt_lastName = nullptr;
    QLineEdit* m_qt_middleName = nullptr;
    QLineEdit* m_qt_email = nullptr;
    QGroupBox* m_qt_login = nullptr;
    QFormLayout* m_qt_loginForm = nullptr;
    QLineEdit* m_qt_userName = nullptr;
    QLineEdit* m_qt_password = nullptr;
    QCheckBox* m_qt_autoGenerateLoginAndPassword = nullptr;
};

#endif // USEREDITORDIALOG_H_
