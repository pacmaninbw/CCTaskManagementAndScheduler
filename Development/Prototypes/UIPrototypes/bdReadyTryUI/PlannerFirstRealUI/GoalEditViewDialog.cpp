#include "GoalEditViewDialog.h"
#include "ui_GoalEditViewDialog.h"

GoalEditViewDialog::GoalEditViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GoalEditViewDialog)
{
    ui->setupUi(this);
}

GoalEditViewDialog::~GoalEditViewDialog()
{
    delete ui;
}
