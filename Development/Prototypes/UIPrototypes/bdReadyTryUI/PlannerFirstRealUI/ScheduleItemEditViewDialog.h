#ifndef SCHEDULEITEMEDITVIEWDIALOG_H
#define SCHEDULEITEMEDITVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class ScheduleItemEditViewDialog;
}

class ScheduleItemEditViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleItemEditViewDialog(QWidget *parent = nullptr);
    ~ScheduleItemEditViewDialog();

private:
    Ui::ScheduleItemEditViewDialog *ui;
};

#endif // SCHEDULEITEMEDITVIEWDIALOG_H
