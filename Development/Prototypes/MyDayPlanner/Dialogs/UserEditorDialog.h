#ifndef USEREDITORDIALOG_H
#define USEREDITORDIALOG_H

class UserModel;

// Project Header Files

// QT Header Files
#include "UserModel.h"
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
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
    void setUpUserEditorDialogUi();
    QGroupBox* setUpUserProfileGB();
    QGroupBox* setUpLoginDataGB();
    QDialogButtonBox* setUpEditUserButtonBox();
    void initAllFieldsFromData();
    bool addUser();
    bool upDateUser();
    void transferAllFieldsToData();

// Model related variables
    std::size_t m_userID;
    std::shared_ptr<UserModel> m_UserData;

// QT Widgets
    QDialogButtonBox* editUserButtonBox = nullptr;
    QVBoxLayout* editUserLayout = nullptr;
    QGroupBox* userProfileGB = nullptr;
    QFormLayout* userProfileForm = nullptr;
    QLineEdit* firstNameLE = nullptr;
    QLineEdit* lastNameLE = nullptr;
    QLineEdit* middleNameLE = nullptr;
    QLineEdit* emailLE = nullptr;
    QGroupBox* loginDataGB = nullptr;
    QFormLayout* loginDataForm = nullptr;
    QLineEdit* userNameLE = nullptr;
    QLineEdit* passwordLE = nullptr;
    QCheckBox* autoGenerateLoginAndPassword = nullptr;

    const int groupBoxSpacing = 60;
    const int emailWidth = 300;
    const int emailCharCount = 50;
};

#endif // USEREDITORDIALOG_H
