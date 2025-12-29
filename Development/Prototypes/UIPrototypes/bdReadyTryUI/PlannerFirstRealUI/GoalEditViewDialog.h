#ifndef GOALEDITVIEWDIALOG_H
#define GOALEDITVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class GoalEditViewDialog;
}

class GoalEditViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoalEditViewDialog(QWidget *parent = nullptr);
    ~GoalEditViewDialog();

private:
    Ui::GoalEditViewDialog *ui;
};

#endif // GOALEDITVIEWDIALOG_H
