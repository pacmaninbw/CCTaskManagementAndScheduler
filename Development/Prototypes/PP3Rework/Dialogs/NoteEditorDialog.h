#ifndef NOTEEDITORDIALOG_H
#define NOTEEDITORDIALOG_H

class NoteModel;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class NoteEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t noteID=0);
    ~NoteEditorDialog();

public Q_SLOTS:
    void accept() override;

private:
    void setUpNoteEditorUI();
    QFormLayout* setUpNoteEditorGBForm();
    QDialogButtonBox* setUpEditNoteButtonBox();
    void limitDialogRowth();

    std::size_t m_userID;
    std::shared_ptr<NoteModel> m_NoteData;
    QDialogButtonBox* buttonBox;
    QGroupBox* editNoteEnterContentGB;
    QFormLayout* noteForm;
    QVBoxLayout* editNoteLayOut;
    QPlainTextEdit* editNoteContentTE;
    int maxGroupBoxHeight;
    int maxButtonBoxHeight;

    const int minNoteContentWidth = 60;
    const int maxNoteContentWidth = 90;
    const int noteLineCount = 5;
    const int marginAndSpacing = 30;
};

#endif // NOTEEDITORDIALOG_H
