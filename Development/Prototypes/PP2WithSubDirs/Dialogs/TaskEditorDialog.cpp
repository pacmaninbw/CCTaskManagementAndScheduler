// Project Header Files
#include "commonQTWidgetsForApp.h"
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

    editTaskMainLayout->addWidget(setUpTaskDescriptionAndStatusGroupBox());

    editTaskMainLayout->addLayout(setUpUserSection());

    editTaskMainLayout->addLayout(setUpDateAndRelatedTasksSection());

    editTaskMainLayout->addLayout(setUpEfforAndPrioritySectionLayout());

    editTaskMainLayout->addWidget(setUpEditTaskButtonBox(), 0, Qt::AlignHCenter);

    editTaskMainLayout->setContentsMargins(20, 20, 20, 20);
    editTaskMainLayout->setSpacing(15);

    setLayout(editTaskMainLayout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    adjustSize();

    QString titleStr = taskID? "Edit Task Dialog" : "Add Task Dialog";
    setWindowTitle(titleStr);
}

QHBoxLayout* TaskEditorDialog::setUpUserSection()
{
    userSectionLayout = new QHBoxLayout;
    userSectionLayout->setObjectName("userSectionLayout");

    userSectionLayout->addWidget(setUpTaskCreatorGroupBox(), 0, Qt::AlignLeft);

    userSectionLayout->addWidget(setUpTaskAssigneeGroupBox(), 0, Qt::AlignRight);

    return userSectionLayout;
}

QGroupBox *TaskEditorDialog::setUpTaskCreatorGroupBox()
{
    editTaskCreatorGB = new QGroupBox("Task Created by:", this);
    editTaskCreatorGB->setObjectName("editTaskCreatorGB");
    editTaskCreatorGB->setAlignment(Qt::AlignHCenter);

    editTaskCreatorForm = cqtfa_FormLayoutWithPolicy("editTaskCreatorForm", editTaskCreatorGB);

    editTaskCreatorFirstNameDisplay =
        cqtfa_LineEditWithWidthAndLength("editTaskCreatorFirstNameDisplay", editTaskCreatorGB);
    editTaskCreatorForm->addRow("First Name:", editTaskCreatorFirstNameDisplay);

    editTaskCreatorLastNameDisplay =
        cqtfa_LineEditWithWidthAndLength("editTaskCreatorLastNameDisplay", editTaskCreatorGB);
    editTaskCreatorForm->addRow("Last Name:", editTaskCreatorLastNameDisplay);

    editTaskCreatorGB->setLayout(editTaskCreatorForm);

    return editTaskCreatorGB;
}

QGroupBox *TaskEditorDialog::setUpTaskAssigneeGroupBox()
{
    editTaskAssignedToGB = new QGroupBox("Task Assigned to:", this);
    editTaskAssignedToGB->setObjectName("editTaskAssignedToGB");
    editTaskAssignedToGB->setAlignment(Qt::AlignHCenter);

    editTaskAssigneeForm = cqtfa_FormLayoutWithPolicy("editTaskAssigneeForm", editTaskAssignedToGB);

    editTaskAssignedToFirstNameDisplay = cqtfa_LineEditWithWidthAndLength(
        "editTaskAssignedToFirstNameDisplay", editTaskAssignedToGB);
    editTaskAssigneeForm->addRow("First Name:", editTaskAssignedToFirstNameDisplay);

    editTaskAssignedToLastName = cqtfa_LineEditWithWidthAndLength(
        "editTaskAssignedToLastName", editTaskAssignedToGB);
    editTaskAssigneeForm->addRow("Last Name:", editTaskAssignedToLastName);

    editTaskChangeAssignedUserPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Change Assigned User", "editTaskChangeAssignedUserPB", editTaskAssignedToGB);
    editTaskAssigneeForm->addWidget(editTaskChangeAssignedUserPB);

    connect(editTaskChangeAssignedUserPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskChangeAssignedUserPB_Clicked);

    editTaskAssignedToGB->setLayout(editTaskAssigneeForm);

    return editTaskAssignedToGB;
}

QHBoxLayout* TaskEditorDialog::setUpDateAndRelatedTasksSection()
{
    DateAndRelatedTasksSection = new QHBoxLayout;
    DateAndRelatedTasksSection->setObjectName("DateAndRelatedTasksSection");

    DateAndRelatedTasksSection->addWidget(setUpTaskDatesGroupBox(), 0, Qt::AlignLeft);

    DateAndRelatedTasksSection->addWidget(setUpRelatedTasksGroupBox(), 0, Qt::AlignRight);

    return DateAndRelatedTasksSection;
}

QGroupBox *TaskEditorDialog::setUpRelatedTasksGroupBox()
{
    editTaskParentTaskGB = new QGroupBox("Related Tasks:", this);
    editTaskParentTaskGB->setObjectName("editTaskParentTaskGB");
    editTaskParentTaskGB->setAlignment(Qt::AlignHCenter);

    editTaskParentForm = cqtfa_FormLayoutWithPolicy("editTaskParentForm", editTaskParentTaskGB);

    editTaskParentTaskDescriptionDisplay = cqtfa_LineEditWithWidthAndLength("", editTaskParentTaskGB);
    editTaskParentForm->addRow("", editTaskParentTaskDescriptionDisplay);

    editTaskSelectParentPB = cqtfa_QTWidgetWithText<QPushButton>("Select Parent Task",
        "editTaskSelectParentPB" , editTaskParentTaskGB);
    editTaskParentForm->addWidget(editTaskSelectParentPB);

    connect(editTaskSelectParentPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskSelectParentPB_Clicked);

    editTaskAddDependenciesPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Add Dependencies", "editTaskAddDependenciesPB", this);
    editTaskParentForm->addWidget(editTaskAddDependenciesPB);

    connect(editTaskAddDependenciesPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskAddDependenciesPB_Clicked);


    editTaskParentTaskGB->setLayout(editTaskParentForm);

    return editTaskParentTaskGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDescriptionAndStatusGroupBox()
{
    editTaskDescriptionAndStatusGB = new QGroupBox("Description and Status:", this);
    editTaskDescriptionAndStatusGB->setObjectName("editTaskDescriptionAndStatusGB");
    editTaskDescriptionAndStatusGB->setAlignment(Qt::AlignHCenter);

    editTaskDescriptionAndStatusForm = cqtfa_FormLayoutWithPolicy(
        "editTaskDescriptionAndStatusForm", editTaskDescriptionAndStatusGB);

    editTaskDescriptionTE = cqtfa_QTWidget<QTextEdit>("editTaskDescriptionTE", editTaskDescriptionAndStatusGB);
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

    editTaskPersonalCB = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "editTaskPersonalCB", this);
    editTaskDescriptionAndStatusForm->addWidget(editTaskPersonalCB);

    editTaskDescriptionAndStatusGB->setLayout(editTaskDescriptionAndStatusForm);


    return editTaskDescriptionAndStatusGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDatesGroupBox()
{
    editTaskDatesGB = new QGroupBox("Task Dates:", this);
    editTaskDatesGB->setObjectName("editTaskDatesGB");
    editTaskDatesGB->setAlignment(Qt::AlignHCenter);

    editTaskDatesForm = cqtfa_FormLayoutWithPolicy("editTaskDatesForm", editTaskDatesGB);

    editTaskDueDateSelectorDE = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "editTaskDueDateSelectorDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Due Date:", editTaskDueDateSelectorDE);

    editTaskScheduledStartDE = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "editTaskScheduledStartDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Scheduled Start:", editTaskScheduledStartDE);

    editTaskExpectedCompletionDE  = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "editTaskExpectedCompletionDE", editTaskDatesGB);
    editTaskDatesForm->addRow("Estimated Completion:", editTaskExpectedCompletionDE);

    editTaskDatesGB->setLayout(editTaskDatesForm);

    return editTaskDatesGB;
}

QHBoxLayout* TaskEditorDialog::setUpEfforAndPrioritySectionLayout()
{
    efforAndPrioritySectionLayout = new QHBoxLayout;
    efforAndPrioritySectionLayout->setObjectName("efforAndPrioritySectionLayout");

    efforAndPrioritySectionLayout->addWidget(setUpTaskEfforGroupBox(), 0, Qt::AlignLeft);

    efforAndPrioritySectionLayout->addWidget(setUpTaskPriorityGroupBox(), 0, Qt::AlignRight);

    return efforAndPrioritySectionLayout;
}

QGroupBox *TaskEditorDialog::setUpTaskEfforGroupBox()
{
    editTaskEffortGB = new QGroupBox("Effort:", this);
    editTaskEffortGB->setObjectName("editTaskEffortGB");
    editTaskEffortGB->setAlignment(Qt::AlignHCenter);

    editTaskEffortForm = cqtfa_FormLayoutWithPolicy("editTaskEffortForm", editTaskEffortGB);

    editTaskEstimatedEffortLE = cqtfa_LineEditWithWidthAndLength(
        "editTaskEstimatedEffortLE", editTaskEffortGB, 113, 5);
    editTaskEffortForm->addRow("Estimated:", editTaskEstimatedEffortLE);

    editTaskActualEffortLE = cqtfa_LineEditWithWidthAndLength(
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

    editTaskPrioritiesForm = cqtfa_FormLayoutWithPolicy("editTaskPrioritiesForm",
        editTaskPrioritiesGB);

    editTaskPriorityGroupLE = cqtfa_LineEditWithWidthAndLength(
        "editTaskPriorityGroupLE", editTaskPrioritiesGB, 113, 5);
    editTaskPrioritiesForm->addRow("Priority Group:", editTaskPriorityGroupLE);

    editTaskPriorityLE = cqtfa_LineEditWithWidthAndLength(
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
