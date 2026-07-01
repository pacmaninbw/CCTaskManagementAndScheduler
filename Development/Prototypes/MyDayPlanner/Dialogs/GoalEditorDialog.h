#ifndef GOALEDITORDIALOG_H_
#define GOALEDITORDIALOG_H_

class UserGoalModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

// Standard C++ Header Files
#include <memory>

class GoalEditorDialog : public BaseObjectEditorDialog
{
public:
    explicit GoalEditorDialog(std::size_t userId, std::size_t goalId, QWidget* parent = nullptr);
    ~GoalEditorDialog();
    virtual void initEditorFieldsFromDataBase() override;

private:
    virtual QGroupBox* setUpEditorDialogForm() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;
    virtual void createSharedPtrDBModelForAddObject() override;

    std::shared_ptr<UserGoalModel> m_ParentGoalData;

    QPlainTextEdit* m_qt_Description = nullptr;
    QLineEdit* m_qt_Priority = nullptr;
    QPushButton* m_qt_SelectParentGoal = nullptr;

    const int goalDescriptionMinCharWidth = 40;
    const int goalDescriptionMaxCharWidth = 80;
    const int goalDescriptionLineCount = 3;
    const int goalPriorityMaxChar = 5;
};

#endif // GOALEDITORDIALOG_H_
