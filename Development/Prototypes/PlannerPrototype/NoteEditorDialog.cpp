// Project Header Files
#include "NoteEditorDialog.h"

// QT Header Files

// Standard C++ Header Files

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
    resize(691, 267);

    QString titleString = noteID? "Edit Note" : "Add Note";

    editNoteEnterContentGB = new QGroupBox(titleString + " Content", this);
    editNoteEnterContentGB->setObjectName("editNoteEnterContentGB");
    editNoteEnterContentGB->setGeometry(QRect(30, 30, 600, 160));

    editNoteContentTE = new QTextEdit(editNoteEnterContentGB);
    editNoteContentTE->setObjectName("editNoteContentTE");
    editNoteContentTE->setGeometry(QRect(10, 30, 581, 121));

    setUpEditNoteButtonBox();

    setWindowTitle(titleString + " Dialog");
}

QDialogButtonBox* NoteEditorDialog::setUpEditNoteButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("editNoteButtonBox"));
    buttonBox->setGeometry(QRect(30, 220, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}
