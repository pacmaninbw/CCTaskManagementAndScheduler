#ifndef TASKEDITORDIALOG_H_
#define TASKEDITORDIALOG_H_

/*
 * Provides an editor for the Todo list and TaskModel items.
 */

class TaskModel;
class UserModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
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
    void on_m_qt_PersonalCB_stateChanged(int newState);
    void on_m_qt_SelectParentPB_Clicked();
    void on_m_qt_AddDependenciesPB_Clicked();
    void on_m_qt_ChangeAssignedUserPB_Clicked();
    void on_m_qt_StatusSelectorCBChanged(int index);

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

    QGroupBox* m_qt_CreatorGB = nullptr;
    QFormLayout* m_qt_CreatorForm = nullptr;
    QLineEdit* m_qt_CreatorFirstNameDisplay = nullptr;
    QLineEdit* m_qt_CreatorLastNameDisplay = nullptr;
    QGroupBox* m_qt_AssignedToGB = nullptr;
    QFormLayout* m_qt_AssigneeForm = nullptr;
    QLineEdit* m_qt_AssignedToFirstNameDisplay = nullptr;
    QLineEdit* m_qt_AssignedToLastName = nullptr;
    QPushButton* m_qt_ChangeAssignedUserPB = nullptr;
    QGroupBox* m_qt_DescriptionAndStatusGB = nullptr;
    QFormLayout* m_qt_DescriptionAndStatusForm = nullptr;
    QPlainTextEdit* m_qt_DescriptionTE = nullptr;
    QComboBox* m_qt_StatusSelectorCB = nullptr;
    QGroupBox* m_qt_ParentTaskGB = nullptr;
    QFormLayout* m_qt_ParentForm = nullptr;
    QLineEdit* m_qt_ParentTaskDescriptionDisplay = nullptr;
    QPushButton* m_qt_SelectParentPB = nullptr;
    QGroupBox* m_qt_DatesGB = nullptr;
    QFormLayout* m_qt_DatesForm = nullptr;
    QDateEdit* m_qt_DueDateSelectorDE = nullptr;
    QDateEdit* m_qt_ScheduledStartDE = nullptr;
    QDateEdit* m_qt_ExpectedCompletionDE = nullptr;
    QGroupBox* m_qt_EffortGB = nullptr;
    QFormLayout* m_qt_EffortForm = nullptr;
    QLineEdit* m_qt_EstimatedEffortLE = nullptr;
    QLineEdit* m_qt_ActualEffortLE = nullptr;
    QGroupBox* m_qt_PrioritiesGB = nullptr;
    QFormLayout* m_qt_PrioritiesForm = nullptr;
    QLineEdit* m_qt_PriorityGroupLE = nullptr;
    QLineEdit* m_qt_PriorityLE = nullptr;
    QCheckBox* m_qt_PersonalCB = nullptr;
    QPushButton* m_qt_AddDependenciesPB = nullptr;
    QHBoxLayout* m_qt_UserSectionLayout = nullptr;
    QHBoxLayout* m_qt_DateAndRelatedTasksSection = nullptr;
    QHBoxLayout* m_qt_EfforAndPrioritySectionLayout = nullptr;

    const int taskDescriptionTEWidth = 700;
    const int taskDescriptionTEHeight = 60;
};

#endif // TASKEDITORDIALOG_H_
