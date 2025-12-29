#ifndef NOTEEDITVIEWDIALOG_H
#define NOTEEDITVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class NoteEditViewDialog;
}

class NoteEditViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditViewDialog(QWidget *parent = nullptr);
    ~NoteEditViewDialog();

private:
    Ui::NoteEditViewDialog *ui;
};

#endif // NOTEEDITVIEWDIALOG_H
