#ifndef TASKEDITVIEWDIALOG_H
#define TASKEDITVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class TaskEditViewDialog;
}

class TaskEditViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskEditViewDialog(QWidget *parent = nullptr);
    ~TaskEditViewDialog();

private:
    Ui::TaskEditViewDialog *ui;
};

#endif // TASKEDITVIEWDIALOG_H
