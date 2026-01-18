#ifndef USEREDITORDIALOG_H
#define USEREDITORDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
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
    explicit UserEditorDialog(QWidget *parent = nullptr, std::size_t userId=0);
    ~UserEditorDialog();

public slots:

signals:

private slots:

private:
    void setUpUserEditorialogUi();
    QGroupBox* setUpUserProfileGB();
    QGroupBox* setUpLoginDataGB();
    QDialogButtonBox* setUpEditUserButtonBox();

// Model related variables
    std::size_t userID;

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

    const int groupBoxSpacing = 60;
};

#endif // USEREDITORDIALOG_H
