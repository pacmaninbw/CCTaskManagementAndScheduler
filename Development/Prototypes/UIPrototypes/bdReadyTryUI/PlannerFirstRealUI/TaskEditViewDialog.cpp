#include "TaskEditViewDialog.h"
#include "ui_TaskEditViewDialog.h"

TaskEditViewDialog::TaskEditViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TaskEditViewDialog)
{
    ui->setupUi(this);
}

TaskEditViewDialog::~TaskEditViewDialog()
{
    delete ui;
}
