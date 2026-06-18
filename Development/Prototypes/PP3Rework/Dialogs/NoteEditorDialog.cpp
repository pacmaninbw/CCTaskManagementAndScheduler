// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "NoteModel.h"
#include "NoteQueryProcessor.h"
#include "NoteEditorDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QMessageBox>

// Standard C++ Header Files

#include "NoteEditorDialog.h"

NoteEditorDialog::NoteEditorDialog(QWidget *parent, std::size_t userId, std::size_t noteToEdit)
    : QDialog(parent),
    m_userID{userId},
    m_DBModelID{noteToEdit},
    m_NoteData{nullptr}
{
    setUpNoteEditorUI();
}

NoteEditorDialog::~NoteEditorDialog()
{
}

void NoteEditorDialog::initEditFieldsFromDB()
{
    if (m_DBModelID != 0)
    {
        NoteQueryProcessor noteQueryProcess;
        m_NoteData = noteQueryProcess.getNoteById(m_DBModelID);
        if (m_NoteData)
        {
            editNoteContentTE->setPlainText(QString::fromStdString(m_NoteData->getContent()));
        }
    }
}

void NoteEditorDialog::accept()
{
    bool updateSuccessful = true;
    if (!m_NoteData)
    {
        m_NoteData = std::make_shared<NoteModel>();
        m_NoteData->setUserId(m_userID);
        m_NoteData->setContent(editNoteContentTE->toPlainText().toStdString());
        updateSuccessful = m_NoteData->insert();
    }
    else
    {
        m_NoteData->setContent(editNoteContentTE->toPlainText().toStdString());
        updateSuccessful = m_NoteData->update();
    }

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "User edit failed.\n";
        errorReport += QString::fromStdString(m_NoteData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }

}

void NoteEditorDialog::handleDelteNote_Clicked()
{
    m_NoteData->hide(m_userID);
}

void NoteEditorDialog::setUpNoteEditorUI()
{
    QString titleString = m_DBModelID? "Edit Note" : "Add Note";

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

    editNoteContentTE = cqtfa_flexiblePlainTextEditEbasedOnCharCount("editNoteContentTE",
        editNoteEnterContentGB, minNoteContentWidth, maxNoteContentWidth, noteLineCount);
    noteForm->addRow("Content:", editNoteContentTE);

    if (m_DBModelID)
    {
        deleteButton = new DeleteItemButton("Note", editNoteEnterContentGB);
        noteForm->addWidget(deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, &NoteEditorDialog::handleDelteNote_Clicked);
    }

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
