#ifndef NOTEEDITORDIALOG_H
#define NOTEEDITORDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class NoteEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t noteId=0);
    ~NoteEditorDialog();

public slots:

signals:

private slots:

private:
    void setUpNoteEditorUI();
    QDialogButtonBox* setUpEditNoteButtonBox();

    std::size_t userID;
    std::size_t noteID;
    QDialogButtonBox* buttonBox;
    QGroupBox* editNoteEnterContentGB;
    QVBoxLayout* editNoteLayOut;
    QTextEdit* editNoteContentTE;

    const int minNoteContentWidth = 60;
    const int maxNoteContentWidth = 90;
    const int noteLineCount = 5;
};

#endif // NOTEEDITORDIALOG_H
