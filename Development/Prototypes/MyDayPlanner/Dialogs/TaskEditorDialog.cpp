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
    m_Creator{creator},
    m_ParentTaskData{nullptr},
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
    if (!m_DBModelID)
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " No Task to Edit";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }

    TaskQueryProcessor taskQueryProcessor;
    TaskModel_shp taskData = taskQueryProcessor.getTaskByTaskID(m_DBModelID);
    if (!taskData)
    {
        QString errorReport = "To Do Item Edit failed.\n";
        errorReport += " To Do Item not found in database";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }

    m_DBObjectModel = std::dynamic_pointer_cast<TaskModel>(taskData);

    initDisplayFields();
    initEditFieldsFromTaskData();
}

void TaskEditorDialog::accept()
{
    std::shared_ptr<TaskModel> errorGenerator = nullptr;

    transferEditorValuesToDBModel();

    bool updateSuccessful = m_DBObjectModel->save();

    if (updateSuccessful)
    {
        if (m_parentTaskUpdated)
        {
            TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);
            // Child database task id may not be correct prior to this in the case of
            // a new task being added.
            m_ParentTaskData->addDependency(taskData->getTaskID());
            updateSuccessful = m_ParentTaskData->update();
        }
        if (!updateSuccessful)
        {
            errorGenerator = m_ParentTaskData;
        }
    }
    else
    {
        errorGenerator = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);
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

void TaskEditorDialog::on_m_qt_PersonalCB_stateChanged(int newState)
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
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);
    taskData->setPersonal(isChecked);
}

void TaskEditorDialog::on_m_qt_SelectParentPB_Clicked()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

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

        m_ParentTaskData = taskQueryProcessor.getTaskByTaskID(parentTaskid);
        if (m_ParentTaskData)
        {
            taskData->setParentTaskID(parentTaskid);
            m_parentTaskUpdated = true;
            m_qt_ParentTaskDescriptionDisplay->setText(QString::fromStdString(m_ParentTaskData->getDescription()));
        }
    }
}

void TaskEditorDialog::on_m_qt_AddDependenciesPB_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Add Dependencies to Task Button");
    msgBox.setText("on_m_qt_AddDependenciesPB_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok); 
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::on_m_qt_ChangeAssignedUserPB_Clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Change Assigned User Button");
    msgBox.setText("on_m_qt_ChangeAssignedUserPB_Clicked NOT IMPLEMENTED");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        // Code to execute when the OK button is clicked
        qDebug("OK was clicked");
    }
}

void TaskEditorDialog::on_m_qt_StatusSelectorCBChanged(int index)
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);
    taskData->setStatus(static_cast<TaskModel::TaskStatus>(index));
}

void TaskEditorDialog::createSharedPtrDBModelForAddObject()
{
    TaskModel_shp taskData = std::make_shared<TaskModel>();
    m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(taskData);
}

void TaskEditorDialog::setUpEditorUI()
{
    m_qt_EditorLayout = new QVBoxLayout(this);
    m_qt_EditorLayout->setObjectName("m_qt_EditorLayout");

    m_qt_EditorLayout->addWidget(setUpTaskDescriptionAndStatusGroupBox());

    m_qt_EditorLayout->addLayout(setUpUserSection());

    m_qt_EditorLayout->addLayout(setUpDateAndRelatedTasksSection());

    m_qt_EditorLayout->addLayout(setUpEfforAndPrioritySectionLayout());

    m_qt_ButtonBox = setUpEditorButtonBox();
    m_qt_ButtonBox->setObjectName("m_qt_ButtonBox");
    m_qt_EditorLayout->addWidget(m_qt_ButtonBox, 0, Qt::AlignHCenter);

    m_qt_EditorLayout->setContentsMargins(20, 20, 20, 20);
    m_qt_EditorLayout->setSpacing(15);

    setLayout(m_qt_EditorLayout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    adjustSize();
}

QHBoxLayout* TaskEditorDialog::setUpUserSection()
{
    m_qt_UserSectionLayout = new QHBoxLayout;
    m_qt_UserSectionLayout->setObjectName("m_qt_UserSectionLayout");

    m_qt_UserSectionLayout->addWidget(setUpTaskCreatorGroupBox(), 0, Qt::AlignLeft);

    m_qt_UserSectionLayout->addWidget(setUpTaskAssigneeGroupBox(), 0, Qt::AlignRight);

    return m_qt_UserSectionLayout;
}

QGroupBox *TaskEditorDialog::setUpTaskCreatorGroupBox()
{
    m_qt_CreatorGB = new QGroupBox("Task Created by:", this);
    m_qt_CreatorGB->setObjectName("m_qt_CreatorGB");
    m_qt_CreatorGB->setAlignment(Qt::AlignHCenter);

    m_qt_CreatorForm = cqtfa_FormLayoutWithPolicy("m_qt_CreatorForm", m_qt_CreatorGB);

    m_qt_CreatorFirstNameDisplay =
        cqtfa_LineEditWithWidthAndLength("m_qt_CreatorFirstNameDisplay", m_qt_CreatorGB);
    m_qt_CreatorForm->addRow("First Name:", m_qt_CreatorFirstNameDisplay);

    m_qt_CreatorLastNameDisplay =
        cqtfa_LineEditWithWidthAndLength("m_qt_CreatorLastNameDisplay", m_qt_CreatorGB);
    m_qt_CreatorForm->addRow("Last Name:", m_qt_CreatorLastNameDisplay);

    m_qt_CreatorGB->setLayout(m_qt_CreatorForm);

    return m_qt_CreatorGB;
}

QGroupBox *TaskEditorDialog::setUpTaskAssigneeGroupBox()
{
    m_qt_AssignedToGB = new QGroupBox("Task Assigned to:", this);
    m_qt_AssignedToGB->setObjectName("m_qt_AssignedToGB");
    m_qt_AssignedToGB->setAlignment(Qt::AlignHCenter);

    m_qt_AssigneeForm = cqtfa_FormLayoutWithPolicy("m_qt_AssigneeForm", m_qt_AssignedToGB);

    m_qt_AssignedToFirstNameDisplay = cqtfa_LineEditWithWidthAndLength(
        "m_qt_AssignedToFirstNameDisplay", m_qt_AssignedToGB);
    m_qt_AssigneeForm->addRow("First Name:", m_qt_AssignedToFirstNameDisplay);

    m_qt_AssignedToLastName = cqtfa_LineEditWithWidthAndLength(
        "m_qt_AssignedToLastName", m_qt_AssignedToGB);
    m_qt_AssigneeForm->addRow("Last Name:", m_qt_AssignedToLastName);

    m_qt_ChangeAssignedUserPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Change Assigned User", "m_qt_ChangeAssignedUserPB", m_qt_AssignedToGB);
    m_qt_AssigneeForm->addWidget(m_qt_ChangeAssignedUserPB);


    m_qt_AssignedToGB->setLayout(m_qt_AssigneeForm);

    return m_qt_AssignedToGB;
}

QHBoxLayout* TaskEditorDialog::setUpDateAndRelatedTasksSection()
{
    m_qt_DateAndRelatedTasksSection = new QHBoxLayout;
    m_qt_DateAndRelatedTasksSection->setObjectName("m_qt_DateAndRelatedTasksSection");

    m_qt_DateAndRelatedTasksSection->addWidget(setUpTaskDatesGroupBox(), 0, Qt::AlignLeft);

    m_qt_DateAndRelatedTasksSection->addWidget(setUpRelatedTasksGroupBox(), 0, Qt::AlignRight);

    return m_qt_DateAndRelatedTasksSection;
}

QGroupBox *TaskEditorDialog::setUpRelatedTasksGroupBox()
{
    m_qt_ParentTaskGB = new QGroupBox("Related Tasks:", this);
    m_qt_ParentTaskGB->setObjectName("m_qt_ParentTaskGB");
    m_qt_ParentTaskGB->setAlignment(Qt::AlignHCenter);

    m_qt_ParentForm = cqtfa_FormLayoutWithPolicy("m_qt_ParentForm", m_qt_ParentTaskGB);

    m_qt_ParentTaskDescriptionDisplay = cqtfa_LineEditWithWidthAndLength("", m_qt_ParentTaskGB);
    m_qt_ParentForm->addRow("", m_qt_ParentTaskDescriptionDisplay);

    m_qt_SelectParentPB = cqtfa_QTWidgetWithText<QPushButton>("Select Parent Task",
        "m_qt_SelectParentPB" , m_qt_ParentTaskGB);
    m_qt_ParentForm->addWidget(m_qt_SelectParentPB);

    m_qt_AddDependenciesPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Add Dependencies", "m_qt_AddDependenciesPB", this);
    m_qt_ParentForm->addWidget(m_qt_AddDependenciesPB);


    m_qt_ParentTaskGB->setLayout(m_qt_ParentForm);

    return m_qt_ParentTaskGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDescriptionAndStatusGroupBox()
{
    m_qt_DescriptionAndStatusGB = new QGroupBox("Description and Status:", this);
    m_qt_DescriptionAndStatusGB->setObjectName("m_qt_DescriptionAndStatusGB");
    m_qt_DescriptionAndStatusGB->setAlignment(Qt::AlignHCenter);

    m_qt_DescriptionAndStatusForm = cqtfa_FormLayoutWithPolicy(
        "m_qt_DescriptionAndStatusForm", m_qt_DescriptionAndStatusGB);

    m_qt_DescriptionTE = cqtfa_QTWidget<QPlainTextEdit>("m_qt_DescriptionTE", m_qt_DescriptionAndStatusGB);
    m_qt_DescriptionTE->resize(taskDescriptionTEWidth, taskDescriptionTEHeight);
    m_qt_DescriptionAndStatusForm->addRow("Task Description:", m_qt_DescriptionTE);

    m_qt_StatusSelectorCB = new QComboBox(m_qt_DescriptionAndStatusGB);
    m_qt_StatusSelectorCB->setObjectName("m_qt_StatusSelectorCB");
    m_qt_StatusSelectorCB->addItem( "Not Started", 0);
    m_qt_StatusSelectorCB->addItem("On Hold", 1);
    m_qt_StatusSelectorCB->addItem("Waiting for Dependency", 2);
    m_qt_StatusSelectorCB->addItem("Work in Progress", 3);
    m_qt_StatusSelectorCB->addItem("Complete", 4);
    m_qt_DescriptionAndStatusForm->addRow("Current Status:", m_qt_StatusSelectorCB);

    m_qt_PersonalCB = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "m_qt_PersonalCB", this);
    m_qt_DescriptionAndStatusForm->addWidget(m_qt_PersonalCB);

    m_qt_DescriptionAndStatusGB->setLayout(m_qt_DescriptionAndStatusForm);

    return m_qt_DescriptionAndStatusGB;
}

QGroupBox *TaskEditorDialog::setUpTaskDatesGroupBox()
{
    m_qt_DatesGB = new QGroupBox("Task Dates:", this);
    m_qt_DatesGB->setObjectName("m_qt_DatesGB");
    m_qt_DatesGB->setAlignment(Qt::AlignHCenter);

    m_qt_DatesForm = cqtfa_FormLayoutWithPolicy("m_qt_DatesForm", m_qt_DatesGB);

    m_qt_DueDateSelectorDE = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "m_qt_DueDateSelectorDE", m_qt_DatesGB);
    m_qt_DatesForm->addRow("Due Date:", m_qt_DueDateSelectorDE);

    m_qt_ScheduledStartDE = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "m_qt_ScheduledStartDE", m_qt_DatesGB);
    m_qt_DatesForm->addRow("Scheduled Start:", m_qt_ScheduledStartDE);

    m_qt_ExpectedCompletionDE  = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "m_qt_ExpectedCompletionDE", m_qt_DatesGB);
    m_qt_DatesForm->addRow("Estimated Completion:", m_qt_ExpectedCompletionDE);

    m_qt_DatesGB->setLayout(m_qt_DatesForm);

    return m_qt_DatesGB;
}

QHBoxLayout* TaskEditorDialog::setUpEfforAndPrioritySectionLayout()
{
    m_qt_EfforAndPrioritySectionLayout = new QHBoxLayout;
    m_qt_EfforAndPrioritySectionLayout->setObjectName("m_qt_EfforAndPrioritySectionLayout");

    m_qt_EfforAndPrioritySectionLayout->addWidget(setUpTaskEfforGroupBox(), 0, Qt::AlignLeft);

    m_qt_EfforAndPrioritySectionLayout->addWidget(setUpTaskPriorityGroupBox(), 0, Qt::AlignRight);

    return m_qt_EfforAndPrioritySectionLayout;
}

QGroupBox *TaskEditorDialog::setUpTaskEfforGroupBox()
{
    m_qt_EffortGB = new QGroupBox("Effort:", this);
    m_qt_EffortGB->setObjectName("m_qt_EffortGB");
    m_qt_EffortGB->setAlignment(Qt::AlignHCenter);

    m_qt_EffortForm = cqtfa_FormLayoutWithPolicy("m_qt_EffortForm", m_qt_EffortGB);

    m_qt_EstimatedEffortLE = cqtfa_LineEditWithWidthAndLength(
        "m_qt_EstimatedEffortLE", m_qt_EffortGB, 113, 5);
    m_qt_EffortForm->addRow("Estimated:", m_qt_EstimatedEffortLE);

    m_qt_ActualEffortLE = cqtfa_LineEditWithWidthAndLength(
        "m_qt_ActualEffortLE", m_qt_EffortGB, 113, 5);
    m_qt_EffortForm->addRow("Actual:", m_qt_ActualEffortLE);

    m_qt_EffortGB->setLayout(m_qt_EffortForm);

    return m_qt_EffortGB;
}

QGroupBox *TaskEditorDialog::setUpTaskPriorityGroupBox()
{
    m_qt_PrioritiesGB = new QGroupBox("Priority:", this);
    m_qt_PrioritiesGB->setObjectName("m_qt_PrioritiesGB");
    m_qt_PrioritiesGB->setAlignment(Qt::AlignHCenter);

    m_qt_PrioritiesForm = cqtfa_FormLayoutWithPolicy("m_qt_PrioritiesForm",
        m_qt_PrioritiesGB);

    m_qt_PriorityGroupLE = cqtfa_LineEditWithWidthAndLength(
        "m_qt_PriorityGroupLE", m_qt_PrioritiesGB, 113, 5);
    m_qt_PrioritiesForm->addRow("Priority Group:", m_qt_PriorityGroupLE);

    m_qt_PriorityLE = cqtfa_LineEditWithWidthAndLength(
        "m_qt_PriorityLE", m_qt_PrioritiesGB, 113, 5);
    m_qt_PrioritiesForm->addRow("Priority:", m_qt_PriorityLE);

    m_qt_PrioritiesGB->setLayout(m_qt_PrioritiesForm);

    return m_qt_PrioritiesGB;
}

void TaskEditorDialog::transferEditorValuesToDBModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

    taskData->setCreatorID(m_Creator->getUserID());
    taskData->setAssignToID(m_Assignee->getUserID());
    taskData->setDescription(m_qt_DescriptionTE->toPlainText().toStdString());
    taskData->setDueDate(qDateToChrono(m_qt_DueDateSelectorDE->date()));
    taskData->setScheduledStart(qDateToChrono(m_qt_ScheduledStartDE->date()));
    taskData->setEstimatedCompletion(qDateToChrono(m_qt_ExpectedCompletionDE->date()));
    transferEffortToModel();
    transferPriorityToModel();
    taskData->setPersonal(m_qt_PersonalCB->isChecked());

    if (m_ParentTaskData)
    {
        taskData->setParentTaskID(m_ParentTaskData->getTaskID());
    }
}

void TaskEditorDialog::transferDBModelDataToEditorFields()
{
    initDisplayFields();
    initEditFieldsFromTaskData();
}

void TaskEditorDialog::initEditFields()
{
    m_qt_CreatorFirstNameDisplay->setText(QString::fromStdString(m_Creator->getFirstName()));
    m_qt_CreatorLastNameDisplay->setText(QString::fromStdString(m_Creator->getLastName()));
    m_qt_AssignedToFirstNameDisplay->setText(QString::fromStdString(m_Creator->getFirstName()));
    m_qt_AssignedToLastName->setText(QString::fromStdString(m_Creator->getLastName()));

    m_Assignee = m_Creator;

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
    if (!m_DBObjectModel)
    {
        return;
    }

    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

    m_Creator = getUserDataFromTaskData(taskData->getCreatorID());
    initUserNameFields(m_qt_CreatorFirstNameDisplay, m_qt_CreatorLastNameDisplay, m_Creator);

    m_Assignee = getUserDataFromTaskData(taskData->getAssignToID());
    initUserNameFields(m_qt_AssignedToFirstNameDisplay, m_qt_AssignedToLastName, m_Assignee);

    std::size_t dbParentTaskId = taskData->getParentTaskID();
    if (dbParentTaskId)
    {
        TaskQueryProcessor taskQueryProcessor;
        TaskModel_shp dbParentTask = taskQueryProcessor.getTaskByTaskID(dbParentTaskId);
        if (dbParentTask)
        {
            m_ParentTaskData = dbParentTask;
//            std::cerr << *m_ParentTaskData << std::endl;
        }
    }
}

void TaskEditorDialog::initEditFieldsFromTaskData()
{
    if (!m_DBObjectModel)
    {
        return;
    }

    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

    m_qt_DescriptionTE->setPlainText(QString::fromStdString(taskData->getDescription()));

    m_qt_DueDateSelectorDE->setDate(initValidDateField(taskData->getDueDate()));
    m_qt_ScheduledStartDE->setDate(initValidDateField(taskData->getScheduledStart()));
    m_qt_ExpectedCompletionDE->setDate(initValidDateField(taskData->getEstimatedCompletion()));

    m_qt_EstimatedEffortLE->setText(QString::fromStdString(std::to_string(taskData->getEstimatedEffort())));
    m_qt_ActualEffortLE->setText(QString::fromStdString(std::to_string(taskData->getactualEffortToDate())));

    m_qt_PriorityGroupLE->setText(QString::fromStdString(std::to_string(taskData->getPriorityGroup())));
    m_qt_PriorityLE->setText(QString::fromStdString(std::to_string(taskData->getPriority())));

    m_qt_PersonalCB->setChecked(taskData->isPersonal());
    m_qt_StatusSelectorCB->setCurrentIndex(static_cast<int>(taskData->getStatus()));

    if (m_ParentTaskData)
    {
        m_qt_ParentTaskDescriptionDisplay->setText(QString::fromStdString(m_ParentTaskData->getDescription()));
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
    connect(m_qt_ChangeAssignedUserPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_m_qt_ChangeAssignedUserPB_Clicked, Qt::UniqueConnection);

    connect(m_qt_SelectParentPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_m_qt_SelectParentPB_Clicked, Qt::UniqueConnection);

    connect(m_qt_AddDependenciesPB, &QPushButton::clicked, this,
            &TaskEditorDialog::on_m_qt_AddDependenciesPB_Clicked, Qt::UniqueConnection);

    connect(m_qt_StatusSelectorCB, &QComboBox::currentIndexChanged, this,
            &TaskEditorDialog::on_m_qt_StatusSelectorCBChanged, Qt::UniqueConnection);

    connect(m_qt_PersonalCB, &QCheckBox::stateChanged, this,
            &TaskEditorDialog::on_m_qt_PersonalCB_stateChanged, Qt::UniqueConnection);
}

void TaskEditorDialog::transferEffortToModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

    bool numberisGood = false;
    unsigned int estimatedEffort = m_qt_EstimatedEffortLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setEstimatedEffort(estimatedEffort);
    }

    numberisGood = false;
    double actualEffort = m_qt_ActualEffortLE->text().toDouble(&numberisGood);
    if (numberisGood)
    {
        taskData->setActualEffortToDate(actualEffort);
    }
}

void TaskEditorDialog::transferPriorityToModel()
{
    TaskModel_shp taskData = std::dynamic_pointer_cast<TaskModel>(m_DBObjectModel);

    bool numberisGood = false;
    unsigned int priorityGroup = m_qt_PriorityGroupLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setPriorityGroup(priorityGroup);
    }
    else
    {
        if (m_qt_PriorityGroupLE->text().length() > 0)
        {
            QChar charPriorityGroup = m_qt_PriorityGroupLE->text()[0];
            taskData->setPriorityGroupC(charPriorityGroup.toLatin1());
        }
    }

    numberisGood = false;
    unsigned int priority = m_qt_PriorityLE->text().toUInt(&numberisGood);
    if (numberisGood)
    {
        taskData->setPriority(priority);
    }
}
