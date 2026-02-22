#ifndef TASKEDITORDIALOG_H
#define TASKEDITORDIALOG_H

// Project Header Files
#include "GuiTaskModel.h"
#include "GuiUserModel.h"

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
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class TaskEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskEditorDialog(QWidget* parent = nullptr, GuiUserModel* creator = nullptr);
    ~TaskEditorDialog();
    void setTaskDataAndInitDisplayFields(GuiTaskModel* taskToEdit);

public slots:
    void accept() override;

signals:

private slots:
    void on_editTaskPersonalCB_stateChanged(int newState);
    void on_editTaskSelectParentPB_Clicked();
    void on_editTaskAddDependenciesPB_Clicked();
    void on_editTaskChangeAssignedUserPB_Clicked();
    void on_editTaskStatusSelectorCBChanged(int index);

private:
    void setUpTaskEditorUI();
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
    QDialogButtonBox* setUpEditTaskButtonBox();
    bool addTask();
    bool updateTask();
    void transferAllFieldsToData();
    void initEditFields();
    QDate initValidDateField(QDate fieldData);
    GuiUserModel* getUserDataFromTaskData(std::size_t dbUserId);
    void initDisplayFields();
    void initEditFieldsFromTaskData();
    void initUserNameFields(QLineEdit* firstNameEditor, QLineEdit* lastNameEditor, GuiUserModel* user);
    void connectEditFieldsToActions();

    GuiUserModel* m_Creator;
    GuiTaskModel* m_TaskData;
    GuiUserModel* m_Assignee;
    GuiTaskModel* m_ParentTaskData;

    QVBoxLayout* editTaskMainLayout;
    QDialogButtonBox* editTaskbuttonBox;
    QGroupBox* editTaskCreatorGB;
    QFormLayout* editTaskCreatorForm;
    QLineEdit* editTaskCreatorFirstNameDisplay;
    QLineEdit* editTaskCreatorLastNameDisplay;
    QGroupBox* editTaskAssignedToGB;
    QFormLayout* editTaskAssigneeForm;
    QLineEdit* editTaskAssignedToFirstNameDisplay;
    QLineEdit* editTaskAssignedToLastName;
    QPushButton* editTaskChangeAssignedUserPB;
    QGroupBox* editTaskDescriptionAndStatusGB;
    QFormLayout* editTaskDescriptionAndStatusForm;
    QTextEdit* editTaskDescriptionTE;
    QComboBox* editTaskStatusSelectorCB;
    QGroupBox* editTaskParentTaskGB;
    QFormLayout* editTaskParentForm;
    QLineEdit* editTaskParentTaskDescriptionDisplay;
    QPushButton* editTaskSelectParentPB;
    QGroupBox* editTaskDatesGB;
    QFormLayout* editTaskDatesForm;
    QDateEdit* editTaskDueDateSelectorDE;
    QDateEdit* editTaskScheduledStartDE;
    QDateEdit* editTaskExpectedCompletionDE;
    QGroupBox* editTaskEffortGB;
    QFormLayout* editTaskEffortForm;
    QLineEdit* editTaskEstimatedEffortLE;
    QLineEdit* editTaskActualEffortLE;
    QGroupBox* editTaskPrioritiesGB;
    QFormLayout* editTaskPrioritiesForm;
    QLineEdit* editTaskPriorityGroupLE;
    QLineEdit* editTaskPriorityLE;
    QCheckBox* editTaskPersonalCB;
    QPushButton* editTaskAddDependenciesPB;
    QHBoxLayout* userSectionLayout;
    QHBoxLayout* DateAndRelatedTasksSection;
    QHBoxLayout* efforAndPrioritySectionLayout;

    const int taskDescriptionTEWidth = 700;
    const int taskDescriptionTEHeight = 60;
};

#endif // TASKEDITORDIALOG_H
