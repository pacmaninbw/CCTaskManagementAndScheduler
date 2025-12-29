#include "UserDashboard.h"
#include "ui_UserDashboard.h"

UserDashboard::UserDashboard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserDashboard)
{
    ui->setupUi(this);
}

UserDashboard::~UserDashboard()
{
    delete ui;
}
