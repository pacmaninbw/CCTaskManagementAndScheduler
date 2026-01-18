#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QApplication>
#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

// Standard C++ Header Files


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

public slots:

signals:

private slots:
    void onactionLoginAsUserPBClicked();

private:
    void setUpLoginDialogUI();

    QGroupBox* userLoginGB;
    QLineEdit* userLoginUserNameLE;
    QLineEdit* userLoginPasswordLE;
    QPushButton* actionLoginAsUserPB;

    const int userLoginDialogWidth = 400;
    const int userLoginDialogHeight = 200;

};

#endif // LOGINDIALOG_H
