#ifndef EDITUSERPROFILEDIALOG_H
#define EDITUSERPROFILEDIALOG_H

// Project Headers
#include "createNamedQTWidget.h"

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
#include <QLineEdit>
#include <QVBoxLayout>

// Standard C++ Headers
class EditUserProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserProfileDialog(QWidget* parent = nullptr);
    ~EditUserProfileDialog();
    void setUserID(std::size_t userId) { userID = userId; }

public slots:

signals:

private slots:

private:
    void setUpEditUserProfileDialogUi();
    QGroupBox* setUpUserProfileGB();
    QGroupBox* setUpLoginDataGB();
    QDialogButtonBox* setUpEditUserButtonBox();
    QFormLayout* createNamedFormLayoutWithPolicy(const char* formName);
    QLineEdit* createNamedLineEditWithWidthAndLength(const char* lineEditName, std::size_t width=200, std::size_t charCount=20);

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

#endif // EDITUSERPROFILEDIALOG_H
