#ifndef TASKEDITORDIALOG_H
#define TASKEDITORDIALOG_H

// Project Header Files

// QT Header Files
#include <QDialog>

// Standard C++ Header Files

class TaskEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskEditorDialog(QWidget *parent = nullptr);
    ~TaskEditorDialog();

private:

};

#endif // TASKEDITORDIALOG_H
