#include "ScheduleItemEditViewDialog.h"
#include "ui_ScheduleItemEditViewDialog.h"

ScheduleItemEditViewDialog::ScheduleItemEditViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScheduleItemEditViewDialog)
{
    ui->setupUi(this);
}

ScheduleItemEditViewDialog::~ScheduleItemEditViewDialog()
{
    delete ui;
}
