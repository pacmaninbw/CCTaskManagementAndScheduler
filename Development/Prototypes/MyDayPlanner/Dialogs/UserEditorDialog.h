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
    void setUpUserEditorDialogUi();
    QGroupBox* setUpUserProfileGB();
    QGroupBox* setUpLoginDataGB();
    QDialogButtonBox* setUpEditUserButtonBox();
    void initAllFieldsFromData();
    bool addUser();
    bool upDateUser();
    void transferAllFieldsToData();

// Model related variables
    std::size_t m_UserID;
    std::shared_ptr<UserModel> m_UserData;

// QT Widgets
    QDialogButtonBox* m_qt_ButtonBox = nullptr;
    QVBoxLayout* m_qt_EditorLayout = nullptr;
    QGroupBox* m_qt_UserProfileGB = nullptr;
    QFormLayout* m_qt_UserProfileForm = nullptr;
    QLineEdit* m_qt_FirstName = nullptr;
    QLineEdit* m_qt_LastName = nullptr;
    QLineEdit* m_qt_MiddleName = nullptr;
    QLineEdit* m_qt_Email = nullptr;
    QGroupBox* m_qt_LoginGB = nullptr;
    QFormLayout* m_qt_LoginForm = nullptr;
    QLineEdit* m_qt_UserName = nullptr;
    QLineEdit* m_qt_Password = nullptr;
    QCheckBox* m_qt_AutoGenerateLoginAndPassword = nullptr;

    const int groupBoxSpacing = 60;
    const int emailWidth = 300;
    const int emailCharCount = 50;
};

#endif // USEREDITORDIALOG_H_
