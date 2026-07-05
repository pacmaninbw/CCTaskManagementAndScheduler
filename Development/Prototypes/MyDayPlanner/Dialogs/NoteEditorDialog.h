#ifndef NOTEEDITORDIALOG_H_
#define NOTEEDITORDIALOG_H_

class NoteModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class NoteEditorDialog : public BaseObjectEditorDialog
{
public:
    explicit NoteEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t noteID=0);
    ~NoteEditorDialog();
    virtual void initEditorFieldsFromDataBase() override;

private:
    const int MinNoteContentWidth = 60;
    const int MaxNoteContentWidth = 90;
    const int NoteLineCount = 5;
    const int MarginAndSpacing = 30;

    virtual QGroupBox* setUpEditorDialogForm() override;
    virtual void createSharedPtrDBModelForAddObject() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;

    QPlainTextEdit* m_qt_content = nullptr;
};

#endif // NOTEEDITORDIALOG_H_
