// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "NoteEditorDialog.h"

// QT Header Files
#include <QFormLayout>

// Standard C++ Header Files
#include <format>
#include <iostream>

#include "NoteEditorDialog.h"

NoteEditorDialog::NoteEditorDialog(QWidget *parent, std::size_t userId, std::size_t noteId)
    : QDialog(parent),
    userID{userId},
    noteID{noteId}
{
    setUpNoteEditorUI();
}

NoteEditorDialog::~NoteEditorDialog()
{
}

void NoteEditorDialog::setUpNoteEditorUI()
{
    QString titleString = noteID? "Edit Note" : "Add Note";

    editNoteLayOut = new QVBoxLayout(this);
    editNoteLayOut->setObjectName("editNoteLayOut");

    editNoteEnterContentGB = new QGroupBox(titleString, this);
    editNoteEnterContentGB->setObjectName("editNoteEnterContentGB");

    editNoteEnterContentGB->setLayout(setUpNoteEditorGBForm());

    editNoteLayOut->addWidget(editNoteEnterContentGB);

    editNoteLayOut->addWidget(setUpEditNoteButtonBox(), 0, Qt::AlignHCenter);

    setLayout(editNoteLayOut);

    setWindowTitle(titleString + " Dialog");

    limitDialogRowth();

    adjustSize();
}

QFormLayout* NoteEditorDialog::setUpNoteEditorGBForm()
{
    noteForm = cqtfa_FormLayoutWithPolicy("NoteForm", editNoteEnterContentGB);

    editNoteContentTE = cqtfa_flexibleTextEditEbasedOnCharCount("editNoteContentTE",
        editNoteEnterContentGB, minNoteContentWidth, maxNoteContentWidth, noteLineCount);
    noteForm->addRow("Content:", editNoteContentTE);

    editNoteEnterContentGB->setLayout(noteForm);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(noteForm);

    return noteForm;
}

QDialogButtonBox *NoteEditorDialog::setUpEditNoteButtonBox()
{
    buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("editNoteButtonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    maxButtonBoxHeight = buttonBox->height() + marginAndSpacing;

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void NoteEditorDialog::limitDialogRowth()
{
    editNoteEnterContentGB->setMaximumHeight(maxGroupBoxHeight);
    editNoteEnterContentGB->setMaximumWidth(cqtfa_getFormLayoutMaxWidth(noteForm) + marginAndSpacing);

    buttonBox->setMaximumHeight(maxButtonBoxHeight);

    int maxDialogWidth = editNoteEnterContentGB->maximumWidth() + marginAndSpacing;

    setMaximumWidth(maxDialogWidth);

    setMaximumHeight(maxGroupBoxHeight + maxButtonBoxHeight + marginAndSpacing);
}
