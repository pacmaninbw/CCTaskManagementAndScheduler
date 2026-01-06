// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "NoteEditorDialog.h"

// QT Header Files
#include <QFormLayout>

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

    editNoteLayOut = new QVBoxLayout(this);
    editNoteLayOut->setObjectName("editNoteLayOut");

    editNoteContentTE = cqtfa_flixbleTextEditEbasedOnCharCount("editNoteContentTE", nullptr, 60, 90, 4);

    QFormLayout* noteForm = cqtfa_FormLayoutWithPolicy("NoteForm", nullptr);

    noteForm->addRow("Content:", editNoteContentTE);

    editNoteLayOut->addLayout(noteForm);

    editNoteLayOut->addWidget(setUpEditNoteButtonBox(), 0, Qt::AlignHCenter);

    setLayout(editNoteLayOut);

    QString titleString = noteID? "Edit Note" : "Add Note";

    setWindowTitle(titleString + " Dialog");

    adjustSize();
}

QDialogButtonBox* NoteEditorDialog::setUpEditNoteButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setObjectName(QString::fromUtf8("editNoteButtonBox"));
    buttonBox->setGeometry(QRect(30, 30, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}
