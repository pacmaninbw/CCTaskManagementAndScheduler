#ifndef USERLOGINDIALOG_H
#define USERLOGINDIALOG_H

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

class UserLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserLoginDialog(QWidget *parent = nullptr);
    ~UserLoginDialog();

public slots:

signals:

private slots:

private:
    QDialogButtonBox* editUserButtonBox;
    QGroupBox* userLoginGB;
    QFormLayout* userLoginFormLayout;
    QLineEdit* userLoginUserNameLE;
    QLineEdit* userLoginPasswordLE;

};

#endif // USERLOGINDIALOG_H
