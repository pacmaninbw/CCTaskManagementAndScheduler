#ifndef NOTEEDITORDIALOG_H
#define NOTEEDITORDIALOG_H

// Project Header Files
#include "GuiNoteModel.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class NoteEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, GuiNoteModel* noteToEdit=nullptr);
    ~NoteEditorDialog();

public slots:
    void accept() override;

signals:

private slots:

private:
    void setUpNoteEditorUI();
    QFormLayout* setUpNoteEditorGBForm();
    QDialogButtonBox* setUpEditNoteButtonBox();
    void limitDialogRowth();

    std::size_t m_userID;
    GuiNoteModel* m_NoteData;
    QDialogButtonBox* buttonBox;
    QGroupBox* editNoteEnterContentGB;
    QFormLayout* noteForm;
    QVBoxLayout* editNoteLayOut;
    QTextEdit* editNoteContentTE;
    int maxGroupBoxHeight;
    int maxButtonBoxHeight;

    const int minNoteContentWidth = 60;
    const int maxNoteContentWidth = 90;
    const int noteLineCount = 5;
    const int marginAndSpacing = 30;
};

#endif // NOTEEDITORDIALOG_H
