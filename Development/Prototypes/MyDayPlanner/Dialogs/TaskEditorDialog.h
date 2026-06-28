#ifndef TASKEDITORDIALOG_H
#define TASKEDITORDIALOG_H

class TaskModel;
class UserModel;
// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <memory>

class TaskEditorDialog : public BaseObjectEditorDialog
{
    public:
    explicit TaskEditorDialog(QWidget* parent = nullptr, std::shared_ptr<UserModel> creator = nullptr, std::size_t taskToEditId = 0);
    ~TaskEditorDialog();
    virtual void initEditorFieldsFromDataBase() override;

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void on_editTaskPersonalCB_stateChanged(int newState);
    void on_editTaskSelectParentPB_Clicked();
    void on_editTaskAddDependenciesPB_Clicked();
    void on_editTaskChangeAssignedUserPB_Clicked();
    void on_editTaskStatusSelectorCBChanged(int index);

private:
    virtual void createSharedPtrDBModelForAddObject() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;

    virtual void setUpEditorUI() override;
    QHBoxLayout* setUpUserSection();
    QGroupBox* setUpTaskCreatorGroupBox();
    QGroupBox* setUpTaskAssigneeGroupBox();
    QHBoxLayout* setUpDateAndRelatedTasksSection();
    QGroupBox* setUpRelatedTasksGroupBox();
    QGroupBox* setUpTaskDescriptionAndStatusGroupBox();
    QGroupBox* setUpTaskDatesGroupBox();
    QHBoxLayout* setUpEfforAndPrioritySectionLayout();
    QGroupBox* setUpTaskEfforGroupBox();
    QGroupBox* setUpTaskPriorityGroupBox();
    void initEditFields();
    QDate initValidDateField(QDate fieldData);
    std::shared_ptr<UserModel> getUserDataFromTaskData(std::size_t dbUserId);
    void initDisplayFields();
    void initEditFieldsFromTaskData();
    void initUserNameFields(QLineEdit* firstNameEditor, QLineEdit* lastNameEditor, std::shared_ptr<UserModel> user);
    void connectEditFieldsToActions();
    void transferEffortToModel();
    void transferPriorityToModel();

    std::shared_ptr<UserModel> m_Creator;
    std::shared_ptr<UserModel> m_Assignee;
    std::shared_ptr<TaskModel> m_ParentTaskData;
    bool m_parentTaskUpdated;

    QGroupBox* editTaskCreatorGB = nullptr;
    QFormLayout* editTaskCreatorForm = nullptr;
    QLineEdit* editTaskCreatorFirstNameDisplay = nullptr;
    QLineEdit* editTaskCreatorLastNameDisplay = nullptr;
    QGroupBox* editTaskAssignedToGB = nullptr;
    QFormLayout* editTaskAssigneeForm = nullptr;
    QLineEdit* editTaskAssignedToFirstNameDisplay = nullptr;
    QLineEdit* editTaskAssignedToLastName = nullptr;
    QPushButton* editTaskChangeAssignedUserPB = nullptr;
    QGroupBox* editTaskDescriptionAndStatusGB = nullptr;
    QFormLayout* editTaskDescriptionAndStatusForm = nullptr;
    QPlainTextEdit* editTaskDescriptionTE = nullptr;
    QComboBox* editTaskStatusSelectorCB = nullptr;
    QGroupBox* editTaskParentTaskGB = nullptr;
    QFormLayout* editTaskParentForm = nullptr;
    QLineEdit* editTaskParentTaskDescriptionDisplay = nullptr;
    QPushButton* editTaskSelectParentPB = nullptr;
    QGroupBox* editTaskDatesGB = nullptr;
    QFormLayout* editTaskDatesForm = nullptr;
    QDateEdit* editTaskDueDateSelectorDE = nullptr;
    QDateEdit* editTaskScheduledStartDE = nullptr;
    QDateEdit* editTaskExpectedCompletionDE = nullptr;
    QGroupBox* editTaskEffortGB = nullptr;
    QFormLayout* editTaskEffortForm = nullptr;
    QLineEdit* editTaskEstimatedEffortLE = nullptr;
    QLineEdit* editTaskActualEffortLE = nullptr;
    QGroupBox* editTaskPrioritiesGB = nullptr;
    QFormLayout* editTaskPrioritiesForm = nullptr;
    QLineEdit* editTaskPriorityGroupLE = nullptr;
    QLineEdit* editTaskPriorityLE = nullptr;
    QCheckBox* editTaskPersonalCB = nullptr;
    QPushButton* editTaskAddDependenciesPB = nullptr;
    QHBoxLayout* userSectionLayout = nullptr;
    QHBoxLayout* DateAndRelatedTasksSection = nullptr;
    QHBoxLayout* efforAndPrioritySectionLayout = nullptr;

    const int taskDescriptionTEWidth = 700;
    const int taskDescriptionTEHeight = 60;
};

#endif // TASKEDITORDIALOG_H
