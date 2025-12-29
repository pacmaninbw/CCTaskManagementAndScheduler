#ifndef NOTEEDITORDIALOG_H
#define NOTEEDITORDIALOG_H

// Project Header Files

// QT Header Files
#include <QDialog>

// Standard C++ Header Files

class NoteEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditorDialog(QWidget *parent = nullptr);
    ~NoteEditorDialog();

private:

};

#endif // NOTEEDITORDIALOG_H
