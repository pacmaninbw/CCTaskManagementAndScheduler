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
    if (m_dbModelId != 0)
    {
        NoteQueryProcessor noteQueryProcess;
        NoteModel_shp noteData = noteQueryProcess.getNoteById(m_dbModelId);
        if (noteData)
        {
            m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(noteData);
            transferDBModelDataToEditorFields();
        }
    }
}

QGroupBox *NoteEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* mainEditorGroupBox = new QGroupBox(m_editorTitleString, this);
    m_qt_editorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_qt_editorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_content = cqtfa_flexiblePlainTextEditEbasedOnCharCount("m_qt_content",
        mainEditorGroupBox, MinNoteContentWidth, MaxNoteContentWidth, NoteLineCount);
    m_qt_editorFormLayout->addRow("Content:", m_qt_content);

    m_maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_qt_editorFormLayout);

    mainEditorGroupBox->setLayout(m_qt_editorFormLayout);

    return mainEditorGroupBox;
}

void NoteEditorDialog::createSharedPtrDBModelForAddObject()
{
    NoteModel_shp newNote = std::make_shared<NoteModel>();
    newNote->setUserId(m_userID);
    m_dbObjectModel = newNote;
}

void NoteEditorDialog::transferEditorValuesToDBModel()
{
    if (m_dbObjectModel)
    {
        NoteModel_shp noteData = std::dynamic_pointer_cast<NoteModel>(m_dbObjectModel);
        noteData->setContent(m_qt_content->toPlainText().toStdString());
    }
}

void NoteEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_dbObjectModel)
    {
        NoteModel_shp noteData = std::dynamic_pointer_cast<NoteModel>(m_dbObjectModel);

        m_qt_content->setPlainText(QString::fromStdString(noteData->getContent()));
    }
}
