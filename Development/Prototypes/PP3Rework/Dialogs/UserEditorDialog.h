#ifndef USEREDITORDIALOG_H
#define USEREDITORDIALOG_H

// Project Header Files

// QT Header Files
#include "GuiUserModel.h"
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

class UserEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditorDialog(QWidget *parent = nullptr, GuiUserModel* userData=nullptr);
    ~UserEditorDialog();

public slots:
    void accept() override;

signals:

private slots:

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
    GuiUserModel* m_UserData;

// QT Widgets
    QDialogButtonBox* editUserButtonBox;
    QVBoxLayout* editUserLayout;
    QGroupBox* userProfileGB;
    QFormLayout* userProfileForm;
    QLineEdit* firstNameLE;
    QLineEdit* lastNameLE;
    QLineEdit* middleNameLE;
    QLineEdit* emailLE;
    QGroupBox* loginDataGB;
    QFormLayout* loginDataForm;
    QLineEdit* userNameLE;
    QLineEdit* passwordLE;
    QCheckBox* autoGenerateLoginAndPassword;

    const int groupBoxSpacing = 60;
    const int emailWidth = 300;
    const int emailCharCount = 50;
};

#endif // USEREDITORDIALOG_H
