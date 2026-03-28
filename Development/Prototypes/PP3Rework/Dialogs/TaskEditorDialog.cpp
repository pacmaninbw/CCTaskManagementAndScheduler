// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "stdChronoToQTConversions.h"
#include "TaskEditorDialog.h"
#include "TaskModel.h"
#include "UserModel.h"

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
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

TaskEditorDialog::TaskEditorDialog(QWidget *parent, std::shared_ptr<UserModel> creator)
    : QDialog(parent),
    m_Creator{creator},
    m_TaskData{nullptr},
    m_ParentTaskData{nullptr},
    m_parentTaskUpdated{false}
{
    setUpTaskEditorUI();

    if (creator)
    {
        initEditFields();
    }
}

TaskEditorDialog::~TaskEditorDialog()
{

}

bool TaskEditorDialog::setTaskDataAndInitDisplayFields(std::size_t taskToEditId)
{
    if (!taskToEditId)
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " No Task to Edit";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
        return false;
    }

    m_TaskData = std::make_shared<TaskModel>();
    m_TaskData->setTaskID(taskToEditId);
    if (!m_TaskData->retrieve())
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " To Do Item not found in database";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
        return false;
    }

    initDisplayFields();
    initEditFieldsFromTaskData();
    return true;
}

void TaskEditorDialog::accept()
{
    std::shared_ptr<TaskModel> errorGenerator = nullptr;

    bool updateSuccessful = (m_TaskData->isInDataBase())? updateTask() : addTask();

    if (updateSuccessful)
    {
        if (m_parentTaskUpdated)
        {
            // Child database task id may not be correct prior to this in the case of
            // a new task being added.
            m_ParentTaskData->addDependency(m_TaskData->getTaskID());
            updateSuccessful = m_ParentTaskData->update();
        }
        if (!updateSuccessful)
        {
            errorGenerator = m_ParentTaskData;
        }
    }
    else
    {
        errorGenerator = m_TaskData;
    }

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "Task edit failed.\n";
        errorReport += QString::fromStdString(errorGenerator->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void TaskEditorDialog::on_editTaskPersonalCB_stateChanged(int newState)
{
    bool isChecked = false;
    switch (newState)
    {
        default:
            {
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Personal Checkbox");
                QString myMessage("Invalid state entered ");
                myMessage += QString::number(newState);
                msgBox.setText(myMessage);
                msgBox.setStandardButtons(QMessageBox::Ok); 
                msgBox.setDefaultButton(QMessageBox::Ok);
                
                int ret = msgBox.exec();

                if (ret == QMessageBox::Ok) {
                    // Code to execute when the OK button is clicked
                    qDebug("OK was clicked");
                }
            }
            return;

        case Qt::Checked:
            isChecked = true;
            break;

        case Qt::Unchecked:
            isChecked = false;
            break;    
    }
    m_TaskData->setPersonal(isChecked);
}

void TaskEditorDialog::on_editTaskSelectParentPB_Clicked()
{
    if (m_TaskData->getCreatorID() == 0)
    {
        // If this is a new task that did not retrive the task from the database
        // then update the task before using it in the select parent editor.
        transferAllFieldsToData();
    }

    SelectTaskParentDialog selectParentTask(m_TaskData->getCreatorID(), this);
    selectParentTask.setupDialogUI();

    if (selectParentTask.exec() == QDialog::Accepted)
    {
        std::size_t parentTaskid = selectParentTask.getParentTask();
        m_ParentTaskData = std::make_shared<TaskModel>();
        m_ParentTaskData->setTaskID(parentTaskid);
        if (m_ParentTaskData->retrieve())
        {
            m_TaskData->setParentTaskID(parentTaskid);
            m_parentTaskUpdated = true;
            editTaskParentTaskDescriptionDisplay->setText(QString::fromStdString(m_ParentTaskData->getDescription()));
        }
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

void TaskEditorDialog::on_editTaskStatusSelectorCBChanged(int index)
{
    m_TaskData->setStatus(static_cast<TaskModel::TaskStatus>(index));
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

    QString titleStr = (m_TaskData && m_TaskData->getTaskID() > 0)? "Edit Task Dialog" : "Add Task Dialog";
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

    editTaskAddDependenciesPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Add Dependencies", "editTaskAddDependenciesPB", this);
    editTaskParentForm->addWidget(editTaskAddDependenciesPB);


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

    editTaskDescriptionTE = cqtfa_QTWidget<QPlainTextEdit>("editTaskDescriptionTE", editTaskDescriptionAndStatusGB);
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

    return editTaskbuttonBox;
}

bool TaskEditorDialog::addTask()
{
    transferAllFieldsToData();

    return m_TaskData->insert();
}

bool TaskEditorDialog::updateTask()
{
    transferAllFieldsToData();

    return m_TaskData->update();
}

void TaskEditorDialog::transferAllFieldsToData()
{
    m_TaskData->setCreatorID(m_Creator->getUserID());
    m_TaskData->setAssignToID(m_Assignee->getUserID());
    m_TaskData->setDescription(editTaskDescriptionTE->toPlainText().toStdString());
    m_TaskData->setDueDate(qDateToChrono(editTaskDueDateSelectorDE->date()));
    m_TaskData->setScheduledStart(qDateToChrono(editTaskScheduledStartDE->date()));
    m_TaskData->setEstimatedCompletion(qDateToChrono(editTaskExpectedCompletionDE->date()));
    transferEffortToModel();
    transferPriorityToModel();
    m_TaskData->setPersonal(editTaskPersonalCB->isChecked());

    if (m_ParentTaskData)
    {
        m_TaskData->setParentTaskID(m_ParentTaskData->getTaskID());
    }
/*
 *     QDate m_ActualStartDate;
 *     QDate m_CompletionDate;
 *     QList<std::size_t> m_Depenedencies;
 */
}

void TaskEditorDialog::initEditFields()
{
    editTaskCreatorFirstNameDisplay->setText(QString::fromStdString(m_Creator->getFirstName()));
    editTaskCreatorLastNameDisplay->setText(QString::fromStdString(m_Creator->getLastName()));
    editTaskAssignedToFirstNameDisplay->setText(QString::fromStdString(m_Creator->getFirstName()));
    editTaskAssignedToLastName->setText(QString::fromStdString(m_Creator->getLastName()));

    m_Assignee = m_Creator;

    m_TaskData = std::make_shared<TaskModel>();

    // To prevent any loops caused by updating display fields the connections
    // are implemented after the fields are initialized.
    connectEditFieldsToActions();

    return;
}

QDate TaskEditorDialog::initValidDateField(QDate fieldData)
{
    QDate tempDate(fieldData);
    if (!tempDate.isValid())
    {
        tempDate = QDate::currentDate();
    }

    return tempDate;
}

std::shared_ptr<UserModel> TaskEditorDialog::getUserDataFromTaskData(std::size_t dbUserId)
{
    UserModel_shp newUser = std::make_shared<UserModel>();
    newUser->setUserID(dbUserId);
    newUser->retrieve();

    return newUser;
}

void TaskEditorDialog::initDisplayFields()
{
    if (!m_TaskData)
    {
        return;
    }

    m_Creator = getUserDataFromTaskData(m_TaskData->getCreatorID());
    initUserNameFields(editTaskCreatorFirstNameDisplay, editTaskCreatorLastNameDisplay, m_Creator);

    m_Assignee = getUserDataFromTaskData(m_TaskData->getAssignToID());
    initUserNameFields(editTaskAssignedToFirstNameDisplay, editTaskAssignedToLastName, m_Assignee);

    std::size_t dbParentTaskId = m_TaskData->getParentTaskID();
    if (dbParentTaskId)
    {
        TaskModel_shp dbParentTask = std::make_shared<TaskModel>();
        dbParentTask->setTaskID(dbParentTaskId);
        if (dbParentTask->retrieve())
        {
            m_ParentTaskData = dbParentTask;
//            std::cerr << *m_ParentTaskData << std::endl;
        }
    }
}

void TaskEditorDialog::initEditFieldsFromTaskData()
{
    if (!m_TaskData)
    {
        return;
    }

    editTaskDescriptionTE->setPlainText(QString::fromStdString(m_TaskData->getDescription()));

    editTaskDueDateSelectorDE->setDate(initValidDateField(m_TaskData->getDueDate()));
    editTaskScheduledStartDE->setDate(initValidDateField(m_TaskData->getScheduledStart()));
    editTaskExpectedCompletionDE->setDate(initValidDateField(m_TaskData->getEstimatedCompletion()));

    editTaskEstimatedEffortLE->setText(QString::fromStdString(std::to_string(m_TaskData->getEstimatedEffort())));
    editTaskActualEffortLE->setText(QString::fromStdString(std::to_string(m_TaskData->getactualEffortToDate())));

    editTaskPriorityGroupLE->setText(QString::fromStdString(std::to_string(m_TaskData->getPriorityGroup())));
    editTaskPriorityLE->setText(QString::fromStdString(std::to_string(m_TaskData->getPriority())));

    editTaskPersonalCB->setChecked(m_TaskData->isPersonal());
    editTaskStatusSelectorCB->setCurrentIndex(static_cast<int>(m_TaskData->getStatus()));

    if (m_ParentTaskData)
    {
        editTaskParentTaskDescriptionDisplay->setText(QString::fromStdString(m_ParentTaskData->getDescription()));
    }

    // To prevent any loops caused by updating display fields the connections
    // are implemented after the fields are initialized.
    connectEditFieldsToActions();
}

void TaskEditorDialog::initUserNameFields(QLineEdit *firstNameEditor, QLineEdit *lastNameEditor, std::shared_ptr<UserModel> user)
{
    firstNameEditor->setText(QString::fromStdString(user->getFirstName()));
    lastNameEditor->setText(QString::fromStdString(user->getLastName()));
}

void TaskEditorDialog::connectEditFieldsToActions()
{
    connect(editTaskChangeAssignedUserPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskChangeAssignedUserPB_Clicked, Qt::UniqueConnection);

    connect(editTaskSelectParentPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskSelectParentPB_Clicked, Qt::UniqueConnection);

    connect(editTaskAddDependenciesPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_editTaskAddDependenciesPB_Clicked, Qt::UniqueConnection);

    connect(editTaskStatusSelectorCB, &QComboBox::currentIndexChanged, this,
            &TaskEditorDialog::on_editTaskStatusSelectorCBChanged, Qt::UniqueConnection);

    connect(editTaskPersonalCB, &QCheckBox::stateChanged, this,
            &TaskEditorDialog::on_editTaskPersonalCB_stateChanged, Qt::UniqueConnection);

    connect(editTaskbuttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept, Qt::UniqueConnection);

    connect(editTaskbuttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject, Qt::UniqueConnection);
}

void TaskEditorDialog::transferEffortToModel()
{
    bool numberisGood = false;
    unsigned int estimatedEffort = editTaskEstimatedEffortLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        m_TaskData->setEstimatedEffort(estimatedEffort);
    }

    numberisGood = false;
    double actualEffort = editTaskActualEffortLE->text().toDouble(&numberisGood);
    if (numberisGood)
    {
        m_TaskData->setActualEffortToDate(actualEffort);
    }
}

void TaskEditorDialog::transferPriorityToModel()
{
    bool numberisGood = false;
    unsigned int priorityGroup = editTaskPriorityGroupLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        m_TaskData->setPriorityGroup(priorityGroup);
    }
    else
    {
        if (editTaskPriorityGroupLE->text().length() > 0)
        {
            QChar charPriorityGroup = editTaskPriorityGroupLE->text()[0];
            m_TaskData->setPriorityGroupC(charPriorityGroup.toLatin1());
        }
    }

    numberisGood = false;
    unsigned int priority = editTaskPriorityLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        m_TaskData->setPriority(priority);
    }

}
