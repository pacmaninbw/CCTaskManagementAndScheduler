// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "NoteModel.h"
#include "NoteQueryProcessor.h"
#include "NoteEditorDialog.h"

// QT Header Files
#include <QFormLayout>

// Standard C++ Header Files
#include <memory>

NoteEditorDialog::NoteEditorDialog(QWidget *parent, std::size_t userId, std::size_t noteToEdit)
    : BaseObjectEditorDialog("Note", userId, noteToEdit, parent)
{
    setUpEditorUI();
}

NoteEditorDialog::~NoteEditorDialog()
{
}

void NoteEditorDialog::initEditorFieldsFromDataBase()
{
    if (m_DBModelID != 0)
    {
        NoteQueryProcessor noteQueryProcess;
        NoteModel_shp noteData = noteQueryProcess.getNoteById(m_DBModelID);
        if (noteData)
        {
            m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(noteData);
            transferDBModelDataToEditorFields();
        }
    }
}

QGroupBox *NoteEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* mainEditorGroupBox = new QGroupBox(m_EditorTitleString, this);
    m_Qt_EditorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_Qt_EditorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    editNoteContentTE = cqtfa_flexiblePlainTextEditEbasedOnCharCount("editNoteContentTE",
        mainEditorGroupBox, minNoteContentWidth, maxNoteContentWidth, noteLineCount);
    m_Qt_EditorFormLayout->addRow("Content:", editNoteContentTE);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_Qt_EditorFormLayout);

    mainEditorGroupBox->setLayout(m_Qt_EditorFormLayout);

    return mainEditorGroupBox;
}

void NoteEditorDialog::createSharedPtrDBModelForAddObject()
{
    NoteModel_shp newNote = std::make_shared<NoteModel>();
    newNote->setUserId(m_UserID);
    m_DBObjectModel = newNote;
}

void NoteEditorDialog::transferEditorValuesToDBModel()
{
    if (m_DBObjectModel)
    {
        NoteModel_shp noteData = std::dynamic_pointer_cast<NoteModel>(m_DBObjectModel);
        noteData->setContent(editNoteContentTE->toPlainText().toStdString());
    }
}

void NoteEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_DBObjectModel)
    {
        NoteModel_shp noteData = std::dynamic_pointer_cast<NoteModel>(m_DBObjectModel);

        editNoteContentTE->setPlainText(QString::fromStdString(noteData->getContent()));
    }
}
