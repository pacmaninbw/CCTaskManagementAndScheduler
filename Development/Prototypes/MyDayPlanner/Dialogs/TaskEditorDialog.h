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
    void handlePersonalStateChanged(int newState);
    void handleSelectParent_Clicked();
    void handleAddDependencies_Clicked();
    void handleChangeAssignedUser_Clicked();
    void handleStatusChanged(int index);

private:
    const int TaskDescriptionTEWidth = 700;
    const int TaskDescriptionTEHeight = 60;

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

    std::shared_ptr<UserModel> m_creator;
    std::shared_ptr<UserModel> m_assignee;
    std::shared_ptr<TaskModel> m_parentTaskData;
    bool m_parentTaskUpdated;

    QGroupBox* m_qt_creatorGroup = nullptr;
    QFormLayout* m_qt_creatorForm = nullptr;
    QLineEdit* m_qt_creatorFirstName = nullptr;
    QLineEdit* m_qt_creatorLastName = nullptr;
    QGroupBox* m_qt_assignedToGB = nullptr;
    QFormLayout* m_qt_assigneeForm = nullptr;
    QLineEdit* m_qt_assignedToFirstName = nullptr;
    QLineEdit* m_qt_assignedToLastName = nullptr;
    QPushButton* m_qt_changeAssignedUser = nullptr;
    QGroupBox* m_qt_descriptionAndStatus = nullptr;
    QFormLayout* m_qt_descriptionAndStatusForm = nullptr;
    QPlainTextEdit* m_qt_description = nullptr;
    QComboBox* m_qt_statusSelector = nullptr;
    QGroupBox* m_qt_parentTaskGroup = nullptr;
    QFormLayout* m_qt_parentForm = nullptr;
    QLineEdit* m_qt_parentTaskDescription = nullptr;
    QPushButton* m_qt_selectParent = nullptr;
    QGroupBox* m_qt_dates = nullptr;
    QFormLayout* m_qt_datesForm = nullptr;
    QDateEdit* m_qt_dueDate = nullptr;
    QDateEdit* m_qt_scheduledStart = nullptr;
    QDateEdit* m_qt_expectedCompletion = nullptr;
    QGroupBox* m_qt_effortGB = nullptr;
    QFormLayout* m_qt_effortForm = nullptr;
    QLineEdit* m_qt_estimatedEffort = nullptr;
    QLineEdit* m_qt_actualEffort = nullptr;
    QGroupBox* m_qt_priorities = nullptr;
    QFormLayout* m_qt_prioritiesForm = nullptr;
    QLineEdit* m_qt_priorityCategory = nullptr;
    QLineEdit* m_qt_priority = nullptr;
    QCheckBox* m_qt_personal = nullptr;
    QPushButton* m_qt_addDependencies = nullptr;
    QHBoxLayout* m_qt_userSection = nullptr;
    QHBoxLayout* m_qt_dateAndRelatedTasks = nullptr;
    QHBoxLayout* m_qt_efforAndPriority = nullptr;
};

#endif // TASKEDITORDIALOG_H_
