#ifndef TASKEDITORDIALOG_H
#define TASKEDITORDIALOG_H

// Project Header Files

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
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

class TaskEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t taskId=0);
    ~TaskEditorDialog();

public slots:

signals:

private slots:
    void on_editTaskPersonalCB_stateChanged();
    void on_editTaskSelectParentPB_Clicked();
    void on_editTaskAddDependenciesPB_Clicked();
    void on_editTaskChangeAssignedUserPB_Clicked();

private:
    void setUpTaskEditorUI();
    QGroupBox* setUpTaskCreatorGroupBox();
    QGroupBox* setUpTaskAssigneeGroupBox();
    QGroupBox* setUpRelatedTasksGroupBox();
    QGroupBox* setUpTaskDescriptionAndStatusGroupBox();
    QGroupBox* setUpTaskDatesGroupBox();
    QGroupBox* setUpTaskEfforGroupBox();
    QGroupBox* setUpTaskPriorityGroupBox();
    QDialogButtonBox* setUpEditTaskButtonBox();

    std::size_t userID;
    std::size_t taskID;

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

    const int taskDescriptionTEWidth = 700;
    const int taskDescriptionTEHeight = 60;
};

#endif // TASKEDITORDIALOG_H
