#ifndef NOTEEDITORDIALOG_H
#define NOTEEDITORDIALOG_H

class NoteModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QVariant>
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
    QFormLayout* setUpNoteEditorGBForm();
    virtual QGroupBox* setUpEditorDialogForm() override;
    virtual void createSharedPtrDBModelForAddObject() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;

    QPlainTextEdit* editNoteContentTE = nullptr;

    const int minNoteContentWidth = 60;
    const int maxNoteContentWidth = 90;
    const int noteLineCount = 5;
    const int marginAndSpacing = 30;
};

#endif // NOTEEDITORDIALOG_H
