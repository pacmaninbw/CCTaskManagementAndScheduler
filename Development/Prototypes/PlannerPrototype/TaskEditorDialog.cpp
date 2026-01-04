// Project Header Files
#include "createNamedQTWidget.h"
#include "TaskEditorDialog.h"

// QT Header Files
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
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

TaskEditorDialog::TaskEditorDialog(QWidget *parent, std::size_t userId, std::size_t taskId)
    : QDialog(parent),
    userID{userId},
    taskID{taskId}
{
    setUpTaskEditorUI();
}

TaskEditorDialog::~TaskEditorDialog()
{

}

void TaskEditorDialog::on_editTaskPersonalCB_stateChanged()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Personal Checkbox");
    msgBox.setText("on_editTaskPersonalCB_stateChanged NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok); 
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::on_editTaskSelectParentPB_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Select Parent Task Button");
    msgBox.setText("on_editTaskSelectParentPB_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok); 
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::on_editTaskAddDependenciesPB_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Add Dependencies to Task Button");
    msgBox.setText("on_editTaskAddDependenciesPB_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok); 
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::on_editTaskChangeAssignedUserPB_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Change Assigned User Button");
    msgBox.setText("on_editTaskChangeAssignedUserPB_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::setUpTaskEditorUI()
{

    editTaskMainLayout = new QVBoxLayout(this);
    editTaskMainLayout->setObjectName("editTaskMainLayout");

    editTaskMainLayout->addWidget(setUpTaskCreatorGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpTaskAssigneeGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpTaskDescriptionAndStatusGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpParentTaskGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpTaskDatesGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpTaskEfforGroupBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(setUpTaskPriorityGroupBox(), 0, Qt::AlignHCenter);

    editTaskAddDependenciesPB = createNameQTWidgetWithText<QPushButton>("Add Dependencies", "editTaskAddDependenciesPB", this);

    editTaskPersonalCB = createNameQTWidgetWithText<QCheckBox>("Personal", "editTaskPersonalCB", this);
    editTaskMainLayout->addWidget(editTaskPersonalCB, 0, Qt::AlignHCenter);

    editTaskMainLayout->addWidget(editTaskAddDependenciesPB, 0, Qt::AlignHCenter);
    editTaskMainLayout->addWidget(setUpEditTaskButtonBox(), 0, Qt::AlignHCenter);

    setLayout(editTaskMainLayout);

    adjustSize();

    QString titleStr = taskID? "Edit Task Dialog" : "Add Task Dialog";
    setWindowTitle(titleStr);
}

QGroupBox *TaskEditorDialog::setUpTaskCreatorGroupBox()
{
    editTaskCreatorGB = new QGroupBox("Task Created by:", this);
    editTaskCreatorGB->setObjectName("editTaskCreatorGB");
    editTaskCreatorGB->setAlignment(Qt::AlignHCenter);

    editTaskCreatorForm = createNamedFormLayoutWithPolicy("editTaskCreatorForm", editTaskCreatorGB);

    editTaskCreatorFirstNameDisplay =
        createNamedLineEditWithWidthAndLength("editTaskCreatorFirstNameDisplay", editTaskCreatorGB);
    editTaskCreatorForm->addRow("First Name:", editTaskCreatorFirstNameDisplay);

    editTaskCreatorLastNameDisplay =
        createNamedLineEditWithWidthAndLength("editTaskCreatorLastNameDisplay", editTaskCreatorGB);
    editTaskCreatorForm->addRow("Last Name:", editTaskCreatorLastNameDisplay);

    editTaskCreatorGB->setLayout(editTaskCreatorForm);

    return editTaskCreatorGB;
}

QGroupBox *TaskEditorDialog::setUpTaskAssigneeGroupBox()
{
    editTaskAssignedToGB = new QGroupBox("Task Assigned to:", this);
    editTaskAssignedToGB->setObjectName("editTaskAssignedToGB");
    editTaskAssignedToGB->setAlignment(Qt::AlignHCenter);

    editTaskAssigneeForm = createNamedFormLayoutWithPolicy("editTaskAssigneeForm", editTaskAssignedToGB);

    editTaskAssignedToFirstNameDisplay = createNamedLineEditWithWidthAndLength(
        "editTaskAssignedToFirstNameDisplay", editTaskAssignedToGB);
    editTaskAssigneeForm->addRow("First Name:", editTaskAssignedToFirstNameDisplay);

    editTaskAssignedToLastName = createNamedLineEditWithWidthAndLength(
        "editTaskAssignedToLastName", editTaskAssignedToGB);
    editTaskAssigneeForm->addRow("Last Name:", editTaskAssignedToLastName);

    editTaskChangeAssignedUserPB = createNameQTWidgetWithText<QPushButton>(
        "Change Assigned User", "editTaskChangeAssignedUserPB", editTaskAssignedToGB);
    editTaskAssigneeForm->addWidget(editTaskChangeAssignedUserPB);

    connect(editTaskChangeAssignedUserPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskChangeAssignedUserPB_Clicked);

    editTaskAssignedToGB->setLayout(editTaskAssigneeForm);

    return editTaskAssignedToGB;
}

QGroupBox *TaskEditorDialog::setUpParentTaskGroupBox()
{
    editTaskParentTaskGB = new QGroupBox("Parent Task:", this);
    editTaskParentTaskGB->setObjectName("editTaskParentTaskGB");
    editTaskParentTaskGB->setAlignment(Qt::AlignHCenter);

    editTaskParentForm = createNamedFormLayoutWithPolicy("editTaskParentForm", editTaskParentTaskGB);

    editTaskParentTaskDescriptionDisplay = createNamedLineEditWithWidthAndLength("", editTaskParentTaskGB);
    editTaskParentForm->addRow("", editTaskParentTaskDescriptionDisplay);

    editTaskSelectParentPB = createNameQTWidgetWithText<QPushButton>("Select Parent Task",
        "editTaskSelectParentPB" , editTaskParentTaskGB);
    editTaskParentForm->addWidget(editTaskSelectParentPB);

    connect(editTaskSelectParentPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskSelectParentPB_Clicked);

    editTaskParentTaskGB->setLayout(editTaskParentForm);

    return editTaskParentTaskGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDescriptionAndStatusGroupBox()
{
    editTaskDescriptionAndStatusGB = new QGroupBox("Description and Status:", this);
    editTaskDescriptionAndStatusGB->setObjectName("editTaskDescriptionAndStatusGB");
    editTaskDescriptionAndStatusGB->setAlignment(Qt::AlignHCenter);

    editTaskDescriptionAndStatusForm = createNamedFormLayoutWithPolicy(
        "editTaskDescriptionAndStatusForm", editTaskDescriptionAndStatusGB);

    editTaskDescriptionTE = createNamedQTWidget<QTextEdit>("editTaskDescriptionTE", editTaskDescriptionAndStatusGB);
    editTaskDescriptionTE->resize(taskDescriptionTEWidth, taskDescriptionTEHeight);
    editTaskDescriptionAndStatusForm->addRow("Task Description:", editTaskDescriptionTE);

    editTaskStatusSelectorCB = new QComboBox(editTaskDescriptionAndStatusGB);
    editTaskStatusSelectorCB->setObjectName("editTaskStatusSelectorCB");
    editTaskStatusSelectorCB->addItem( "Not Started", 0);
    editTaskStatusSelectorCB->addItem("On Hold", 1);
    editTaskStatusSelectorCB->addItem("Waiting for Dependency", 2);
    editTaskStatusSelectorCB->addItem("Work in Progress", 3);
    editTaskStatusSelectorCB->addItem("Complete", 4);
    editTaskDescriptionAndStatusForm->addRow("Current Status:", editTaskStatusSelectorCB);

    editTaskDescriptionAndStatusGB->setLayout(editTaskDescriptionAndStatusForm);

    return editTaskDescriptionAndStatusGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDatesGroupBox()
{
    editTaskDatesGB = new QGroupBox("Task Dates:", this);
    editTaskDatesGB->setObjectName("editTaskDatesGB");
    editTaskDatesGB->setAlignment(Qt::AlignHCenter);

    editTaskDatesForm = createNamedFormLayoutWithPolicy("editTaskDatesForm", editTaskDatesGB);

    editTaskDueDateSelectorDE = createNamedDateEditWithCalendarPopUpCurrentDate(
        "editTaskDueDateSelectorDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Due Date:", editTaskDueDateSelectorDE);

    editTaskScheduledStartDE = createNamedDateEditWithCalendarPopUpCurrentDate(
        "editTaskScheduledStartDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Scheduled Start:", editTaskScheduledStartDE);

    editTaskExpectedCompletionDE  = createNamedDateEditWithCalendarPopUpCurrentDate(
        "editTaskExpectedCompletionDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Estimated Completion:", editTaskExpectedCompletionDE);

    editTaskDatesGB->setLayout(editTaskDatesForm);

    return editTaskDatesGB;
}

QGroupBox *TaskEditorDialog::setUpTaskEfforGroupBox()
{
    editTaskEffortGB = new QGroupBox("Effort:", this);
    editTaskEffortGB->setObjectName("editTaskEffortGB");
    editTaskEffortGB->setAlignment(Qt::AlignHCenter);

    editTaskEffortForm = createNamedFormLayoutWithPolicy("editTaskEffortForm", editTaskEffortGB);

    editTaskEstimatedEffortLE = createNamedLineEditWithWidthAndLength(
        "editTaskEstimatedEffortLE", editTaskEffortGB, 113, 5);
    editTaskEffortForm->addRow("Estimated:", editTaskEstimatedEffortLE);

    editTaskActualEffortLE = createNamedLineEditWithWidthAndLength(
        "editTaskActualEffortLE", editTaskEffortGB, 113, 5);
    editTaskEffortForm->addRow("Actual:", editTaskActualEffortLE);

    editTaskEffortGB->setLayout(editTaskEffortForm);

    return editTaskEffortGB;
}

QGroupBox *TaskEditorDialog::setUpTaskPriorityGroupBox()
{
    editTaskPrioritiesGB = new QGroupBox("Priority:", this);
    editTaskPrioritiesGB->setObjectName("editTaskPrioritiesGB");
    editTaskPrioritiesGB->setAlignment(Qt::AlignHCenter);

    editTaskPrioritiesForm = createNamedFormLayoutWithPolicy("editTaskPrioritiesForm",
        editTaskPrioritiesGB);

    editTaskPriorityGroupLE = createNamedLineEditWithWidthAndLength(
        "editTaskPriorityGroupLE", editTaskPrioritiesGB, 113, 5);
    editTaskPrioritiesForm->addRow("Priority Group:", editTaskPriorityGroupLE);

    editTaskPriorityLE = createNamedLineEditWithWidthAndLength(
        "editTaskPriorityLE", editTaskPrioritiesGB, 113, 5);
    editTaskPrioritiesForm->addRow("Priority:", editTaskPriorityLE);

    editTaskPrioritiesGB->setLayout(editTaskPrioritiesForm);

    return editTaskPrioritiesGB;
}

QDialogButtonBox *TaskEditorDialog::setUpEditTaskButtonBox()
{
    editTaskbuttonBox = new QDialogButtonBox(this);

    editTaskbuttonBox->setObjectName(QString::fromUtf8("editTaskbuttonBox"));
    editTaskbuttonBox->setOrientation(Qt::Horizontal);
    editTaskbuttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(editTaskbuttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(editTaskbuttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


    return editTaskbuttonBox;
}
