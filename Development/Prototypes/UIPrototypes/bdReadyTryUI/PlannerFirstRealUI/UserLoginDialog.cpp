#include "UserLoginDialog.h"
#include "ui_UserLoginDialog.h"

UserLoginDialog::UserLoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserLoginDialog)
{
    ui->setupUi(this);
}

UserLoginDialog::~UserLoginDialog()
{
    delete ui;
}
