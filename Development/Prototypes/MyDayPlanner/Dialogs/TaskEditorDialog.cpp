// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "stdChronoToQTConversions.h"
#include "TaskEditorDialog.h"
#include "TaskModel.h"
#include "TaskQueryProcessor.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"

// QT Header Files
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files

TaskEditorDialog::TaskEditorDialog(QWidget *parent, std::shared_ptr<UserModel> creator, std::size_t taskToEdit)
    : BaseObjectEditorDialog("Task", (creator? creator->getUserID() : 0), taskToEdit, parent),
    m_creator{creator},
    m_parentTaskData{nullptr},
    m_parentTaskUpdated{false}
{
    setUpEditorUI();

    if (creator)
    {
        initEditFields();
    }
}

TaskEditorDialog::~TaskEditorDialog()
{

}

void TaskEditorDialog::initEditorFieldsFromDataBase()
{
    if (!m_dbModelId)
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " No Task to Edit";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }

    TaskQueryProcessor taskQueryProcessor;
    TaskModel_shp taskData = taskQueryProcessor.getTaskByTaskID(m_dbModelId);
    if (!taskData)
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " To Do Item not found in database";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }

    m_dbObjectModel = std::dynamic_pointer_cast<TaskModel>(taskData);

    initDisplayFields();
    initEditFieldsFromTaskData();
}

void TaskEditorDialog::accept()
{
    std::shared_ptr<TaskModel> errorGenerator = nullptr;

    transferEditorValuesToDBModel();

    bool updateSuccessful = m_dbObjectModel->save();

    if (updateSuccessful)
    {
        if (m_parentTaskUpdated)
        {
            TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);
            // Child database task id may not be correct prior to this in the case of
            // a new task being added.
            m_parentTaskData->addDependency(taskData->getTaskID());
            updateSuccessful = m_parentTaskData->update();
        }
        if (!updateSuccessful)
        {
            errorGenerator = m_parentTaskData;
        }
    }
    else
    {
        errorGenerator = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);
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

void TaskEditorDialog::handlePersonalStateChanged(int newState)
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
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);
    taskData->setPersonal(isChecked);
}

void TaskEditorDialog::handleSelectParent_Clicked()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    if (taskData->getCreatorID() == 0)
    {
        // If this is a new task that did not retrive the task from the database
        // then update the task before using it in the select parent editor.
        transferEditorValuesToDBModel();
    }

    SelectTaskParentDialog selectParentTask(taskData->getCreatorID(), this);
    selectParentTask.setupDialogUI();

    if (selectParentTask.exec() == QDialog::Accepted)
    {
        std::size_t parentTaskid = selectParentTask.getParentTask();
        TaskQueryProcessor taskQueryProcessor;

        m_parentTaskData = taskQueryProcessor.getTaskByTaskID(parentTaskid);
        if (m_parentTaskData)
        {
            taskData->setParentTaskID(parentTaskid);
            m_parentTaskUpdated = true;
            m_qt_parentTaskDescription->setText(QString::fromStdString(m_parentTaskData->getDescription()));
        }
    }
}

void TaskEditorDialog::handleAddDependencies_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Add Dependencies to Task Button");
    msgBox.setText("handleAddDependencies_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok); 
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::handleChangeAssignedUser_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Change Assigned User Button");
    msgBox.setText("handleChangeAssignedUser_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::handleStatusChanged(int index)
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);
    taskData->setStatus(static_cast<TaskModel::TaskStatus>(index));
}

void TaskEditorDialog::createSharedPtrDBModelForAddObject()
{
    TaskModel_shp taskData = std::make_shared<TaskModel>();
    m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(taskData);
}

void TaskEditorDialog::setUpEditorUI()
{
    m_qt_editorLayout = new QVBoxLayout(this);
    m_qt_editorLayout->setObjectName("m_qt_editorLayout");

    m_qt_editorLayout->addWidget(setUpTaskDescriptionAndStatusGroupBox());

    m_qt_editorLayout->addLayout(setUpUserSection());

    m_qt_editorLayout->addLayout(setUpDateAndRelatedTasksSection());

    m_qt_editorLayout->addLayout(setUpEfforAndPrioritySectionLayout());

    m_qt_buttonBox = setUpEditorButtonBox();
    m_qt_buttonBox->setObjectName("m_qt_buttonBox");
    m_qt_editorLayout->addWidget(m_qt_buttonBox, 0, Qt::AlignHCenter);

    m_qt_editorLayout->setContentsMargins(20, 20, 20, 20);
    m_qt_editorLayout->setSpacing(15);

    setLayout(m_qt_editorLayout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    adjustSize();
}

QHBoxLayout* TaskEditorDialog::setUpUserSection()
{
    m_qt_userSection = new QHBoxLayout;
    m_qt_userSection->setObjectName("m_qt_userSection");

    m_qt_userSection->addWidget(setUpTaskCreatorGroupBox(), 0, Qt::AlignLeft);

    m_qt_userSection->addWidget(setUpTaskAssigneeGroupBox(), 0, Qt::AlignRight);

    return m_qt_userSection;
}

QGroupBox *TaskEditorDialog::setUpTaskCreatorGroupBox()
{
    m_qt_creatorGroup = new QGroupBox("Task Created by:", this);
    m_qt_creatorGroup->setObjectName("m_qt_creatorGroup");
    m_qt_creatorGroup->setAlignment(Qt::AlignHCenter);

    m_qt_creatorForm = cqtfa_FormLayoutWithPolicy("m_qt_creatorForm", m_qt_creatorGroup);

    m_qt_creatorFirstName = cqtfa_LineEditWithWidthAndLength("m_qt_creatorFirstName", m_qt_creatorGroup);
    m_qt_creatorForm->addRow("First Name:", m_qt_creatorFirstName);

    m_qt_creatorLastName = cqtfa_LineEditWithWidthAndLength("m_qt_creatorLastName", m_qt_creatorGroup);
    m_qt_creatorForm->addRow("Last Name:", m_qt_creatorLastName);

    m_qt_creatorGroup->setLayout(m_qt_creatorForm);

    return m_qt_creatorGroup;
}

QGroupBox *TaskEditorDialog::setUpTaskAssigneeGroupBox()
{
    m_qt_assignedToGB = new QGroupBox("Task Assigned to:", this);
    m_qt_assignedToGB->setObjectName("m_qt_assignedToGB");
    m_qt_assignedToGB->setAlignment(Qt::AlignHCenter);

    m_qt_assigneeForm = cqtfa_FormLayoutWithPolicy("m_qt_assigneeForm", m_qt_assignedToGB);

    m_qt_assignedToFirstName = cqtfa_LineEditWithWidthAndLength("m_qt_assignedToFirstName", m_qt_assignedToGB);
    m_qt_assigneeForm->addRow("First Name:", m_qt_assignedToFirstName);

    m_qt_assignedToLastName = cqtfa_LineEditWithWidthAndLength("m_qt_assignedToLastName", m_qt_assignedToGB);
    m_qt_assigneeForm->addRow("Last Name:", m_qt_assignedToLastName);

    m_qt_changeAssignedUser = cqtfa_QTWidgetWithText<QPushButton>(
        "Change Assigned User", "m_qt_changeAssignedUser", m_qt_assignedToGB);
    m_qt_assigneeForm->addWidget(m_qt_changeAssignedUser);


    m_qt_assignedToGB->setLayout(m_qt_assigneeForm);

    return m_qt_assignedToGB;
}

QHBoxLayout* TaskEditorDialog::setUpDateAndRelatedTasksSection()
{
    m_qt_dateAndRelatedTasks = new QHBoxLayout;
    m_qt_dateAndRelatedTasks->setObjectName("m_qt_dateAndRelatedTasks");

    m_qt_dateAndRelatedTasks->addWidget(setUpTaskDatesGroupBox(), 0, Qt::AlignLeft);

    m_qt_dateAndRelatedTasks->addWidget(setUpRelatedTasksGroupBox(), 0, Qt::AlignRight);

    return m_qt_dateAndRelatedTasks;
}

QGroupBox *TaskEditorDialog::setUpRelatedTasksGroupBox()
{
    m_qt_parentTaskGroup = new QGroupBox("Related Tasks:", this);
    m_qt_parentTaskGroup->setObjectName("m_qt_parentTaskGroup");
    m_qt_parentTaskGroup->setAlignment(Qt::AlignHCenter);

    m_qt_parentForm = cqtfa_FormLayoutWithPolicy("m_qt_parentForm", m_qt_parentTaskGroup);

    m_qt_parentTaskDescription = cqtfa_LineEditWithWidthAndLength("", m_qt_parentTaskGroup);
    m_qt_parentForm->addRow("", m_qt_parentTaskDescription);

    m_qt_selectParent = cqtfa_QTWidgetWithText<QPushButton>("Select Parent Task", "m_qt_selectParent" , m_qt_parentTaskGroup);
    m_qt_parentForm->addWidget(m_qt_selectParent);

    m_qt_addDependencies = cqtfa_QTWidgetWithText<QPushButton>("Add Dependencies", "m_qt_addDependencies", this);
    m_qt_parentForm->addWidget(m_qt_addDependencies);

    m_qt_parentTaskGroup->setLayout(m_qt_parentForm);

    return m_qt_parentTaskGroup;
}

QGroupBox *TaskEditorDialog::setUpTaskDescriptionAndStatusGroupBox()
{
    m_qt_descriptionAndStatus = new QGroupBox("Description and Status:", this);
    m_qt_descriptionAndStatus->setObjectName("m_qt_descriptionAndStatus");
    m_qt_descriptionAndStatus->setAlignment(Qt::AlignHCenter);

    m_qt_descriptionAndStatusForm = cqtfa_FormLayoutWithPolicy(
        "m_qt_descriptionAndStatusForm", m_qt_descriptionAndStatus);

    m_qt_description = cqtfa_QTWidget<QPlainTextEdit>("m_qt_description", m_qt_descriptionAndStatus);
    m_qt_description->resize(TaskDescriptionTEWidth, TaskDescriptionTEHeight);
    m_qt_descriptionAndStatusForm->addRow("Task Description:", m_qt_description);

    m_qt_statusSelector = new QComboBox(m_qt_descriptionAndStatus);
    m_qt_statusSelector->setObjectName("m_qt_statusSelector");
    m_qt_statusSelector->addItem("Not Started", 0);
    m_qt_statusSelector->addItem("On Hold", 1);
    m_qt_statusSelector->addItem("Waiting for Dependency", 2);
    m_qt_statusSelector->addItem("Work in Progress", 3);
    m_qt_statusSelector->addItem("Complete", 4);
    m_qt_descriptionAndStatusForm->addRow("Current Status:", m_qt_statusSelector);

    m_qt_personal = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "m_qt_personal", this);
    m_qt_descriptionAndStatusForm->addWidget(m_qt_personal);

    m_qt_descriptionAndStatus->setLayout(m_qt_descriptionAndStatusForm);

    return m_qt_descriptionAndStatus;
}

QGroupBox *TaskEditorDialog::setUpTaskDatesGroupBox()
{
    m_qt_dates = new QGroupBox("Task Dates:", this);
    m_qt_dates->setObjectName("m_qt_dates");
    m_qt_dates->setAlignment(Qt::AlignHCenter);

    m_qt_datesForm = cqtfa_FormLayoutWithPolicy("m_qt_datesForm", m_qt_dates);

    m_qt_dueDate = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_dueDate", m_qt_dates);
    m_qt_datesForm->addRow("Due Date:", m_qt_dueDate);

    m_qt_scheduledStart = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_scheduledStart", m_qt_dates);
    m_qt_datesForm->addRow("Scheduled Start:", m_qt_scheduledStart);

    m_qt_expectedCompletion  = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_expectedCompletion", m_qt_dates);
    m_qt_datesForm->addRow("Estimated Completion:", m_qt_expectedCompletion);

    m_qt_dates->setLayout(m_qt_datesForm);

    return m_qt_dates;
}

QHBoxLayout* TaskEditorDialog::setUpEfforAndPrioritySectionLayout()
{
    m_qt_efforAndPriority = new QHBoxLayout;
    m_qt_efforAndPriority->setObjectName("m_qt_efforAndPriority");

    m_qt_efforAndPriority->addWidget(setUpTaskEfforGroupBox(), 0, Qt::AlignLeft);

    m_qt_efforAndPriority->addWidget(setUpTaskPriorityGroupBox(), 0, Qt::AlignRight);

    return m_qt_efforAndPriority;
}

QGroupBox *TaskEditorDialog::setUpTaskEfforGroupBox()
{
    m_qt_effortGB = new QGroupBox("Effort:", this);
    m_qt_effortGB->setObjectName("m_qt_effortGB");
    m_qt_effortGB->setAlignment(Qt::AlignHCenter);

    m_qt_effortForm = cqtfa_FormLayoutWithPolicy("m_qt_effortForm", m_qt_effortGB);

    m_qt_estimatedEffort = cqtfa_LineEditWithWidthAndLength("m_qt_estimatedEffort", m_qt_effortGB, 113, 5);
    m_qt_effortForm->addRow("Estimated:", m_qt_estimatedEffort);

    m_qt_actualEffort = cqtfa_LineEditWithWidthAndLength("m_qt_actualEffort", m_qt_effortGB, 113, 5);
    m_qt_effortForm->addRow("Actual:", m_qt_actualEffort);

    m_qt_effortGB->setLayout(m_qt_effortForm);

    return m_qt_effortGB;
}

QGroupBox *TaskEditorDialog::setUpTaskPriorityGroupBox()
{
    m_qt_priorities = new QGroupBox("Priority:", this);
    m_qt_priorities->setObjectName("m_qt_priorities");
    m_qt_priorities->setAlignment(Qt::AlignHCenter);

    m_qt_prioritiesForm = cqtfa_FormLayoutWithPolicy("m_qt_prioritiesForm", m_qt_priorities);

    m_qt_priorityCategory = cqtfa_LineEditWithWidthAndLength("m_qt_priorityCategory", m_qt_priorities, 113, 5);
    m_qt_prioritiesForm->addRow("Priority Group:", m_qt_priorityCategory);

    m_qt_priority = cqtfa_LineEditWithWidthAndLength("m_qt_priority", m_qt_priorities, 113, 5);
    m_qt_prioritiesForm->addRow("Priority:", m_qt_priority);

    m_qt_priorities->setLayout(m_qt_prioritiesForm);

    return m_qt_priorities;
}

void TaskEditorDialog::transferEditorValuesToDBModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    taskData->setCreatorID(m_creator->getUserID());
    taskData->setAssignToID(m_assignee->getUserID());
    taskData->setDescription(m_qt_description->toPlainText().toStdString());
    taskData->setDueDate(qDateToChrono(m_qt_dueDate->date()));
    taskData->setScheduledStart(qDateToChrono(m_qt_scheduledStart->date()));
    taskData->setEstimatedCompletion(qDateToChrono(m_qt_expectedCompletion->date()));
    transferEffortToModel();
    transferPriorityToModel();
    taskData->setPersonal(m_qt_personal->isChecked());

    if (m_parentTaskData)
    {
        taskData->setParentTaskID(m_parentTaskData->getTaskID());
    }
}

void TaskEditorDialog::transferDBModelDataToEditorFields()
{
    initDisplayFields();
    initEditFieldsFromTaskData();
}

void TaskEditorDialog::initEditFields()
{
    m_qt_creatorFirstName->setText(QString::fromStdString(m_creator->getFirstName()));
    m_qt_creatorLastName->setText(QString::fromStdString(m_creator->getLastName()));
    m_qt_assignedToFirstName->setText(QString::fromStdString(m_creator->getFirstName()));
    m_qt_assignedToLastName->setText(QString::fromStdString(m_creator->getLastName()));

    m_assignee = m_creator;

    createSharedPtrDBModelForAddObject();

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
    UserQueryProcessor userQueryProcessor;
    UserModel_shp newUser = userQueryProcessor.getUserByID(dbUserId);

    return newUser;
}

void TaskEditorDialog::initDisplayFields()
{
    if (!m_dbObjectModel)
    {
        return;
    }

    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    m_creator = getUserDataFromTaskData(taskData->getCreatorID());
    initUserNameFields(m_qt_creatorFirstName, m_qt_creatorLastName, m_creator);

    m_assignee = getUserDataFromTaskData(taskData->getAssignToID());
    initUserNameFields(m_qt_assignedToFirstName, m_qt_assignedToLastName, m_assignee);

    std::size_t dbParentTaskId = taskData->getParentTaskID();
    if (dbParentTaskId)
    {
        TaskQueryProcessor taskQueryProcessor;
        TaskModel_shp dbParentTask = taskQueryProcessor.getTaskByTaskID(dbParentTaskId);
        if (dbParentTask)
        {
            m_parentTaskData = dbParentTask;
        }
    }
}

void TaskEditorDialog::initEditFieldsFromTaskData()
{
    if (!m_dbObjectModel)
    {
        return;
    }

    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    m_qt_description->setPlainText(QString::fromStdString(taskData->getDescription()));

    m_qt_dueDate->setDate(initValidDateField(taskData->getDueDate()));
    m_qt_scheduledStart->setDate(initValidDateField(taskData->getScheduledStart()));
    m_qt_expectedCompletion->setDate(initValidDateField(taskData->getEstimatedCompletion()));

    m_qt_estimatedEffort->setText(QString::fromStdString(std::to_string(taskData->getEstimatedEffort())));
    m_qt_actualEffort->setText(QString::fromStdString(std::to_string(taskData->getactualEffortToDate())));

    m_qt_priorityCategory->setText(QString::fromStdString(std::to_string(taskData->getPriorityGroup())));
    m_qt_priority->setText(QString::fromStdString(std::to_string(taskData->getPriority())));

    m_qt_personal->setChecked(taskData->isPersonal());
    m_qt_statusSelector->setCurrentIndex(static_cast<int>(taskData->getStatus()));

    if (m_parentTaskData)
    {
        m_qt_parentTaskDescription->setText(QString::fromStdString(m_parentTaskData->getDescription()));
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
    connect(m_qt_changeAssignedUser, &QPushButton::clicked, this,
        &TaskEditorDialog::handleChangeAssignedUser_Clicked, Qt::UniqueConnection);

    connect(m_qt_selectParent, &QPushButton::clicked, this,
        &TaskEditorDialog::handleSelectParent_Clicked, Qt::UniqueConnection);

    connect(m_qt_addDependencies, &QPushButton::clicked, this,
        &TaskEditorDialog::handleAddDependencies_Clicked, Qt::UniqueConnection);

    connect(m_qt_statusSelector, &QComboBox::currentIndexChanged, this,
        &TaskEditorDialog::handleStatusChanged, Qt::UniqueConnection);

    connect(m_qt_personal, &QCheckBox::stateChanged, this,
        &TaskEditorDialog::handlePersonalStateChanged, Qt::UniqueConnection);
}

void TaskEditorDialog::transferEffortToModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    bool numberisGood = false;
    unsigned int estimatedEffort = m_qt_estimatedEffort->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setEstimatedEffort(estimatedEffort);
    }

    numberisGood = false;
    double actualEffort = m_qt_actualEffort->text().toDouble(&numberisGood);
    if (numberisGood)
    {
        taskData->setActualEffortToDate(actualEffort);
    }
}

void TaskEditorDialog::transferPriorityToModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_dbObjectModel);

    bool numberisGood = false;
    unsigned int priorityGroup = m_qt_priorityCategory->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setPriorityGroup(priorityGroup);
    }
    else
    {
        if (m_qt_priorityCategory->text().length() > 0)
        {
            QChar charPriorityGroup = m_qt_priorityCategory->text()[0];
            taskData->setPriorityGroupC(charPriorityGroup.toLatin1());
        }
    }

    numberisGood = false;
    unsigned int priority = m_qt_priority->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setPriority(priority);
    }
}
