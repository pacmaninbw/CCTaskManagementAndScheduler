
// Project Header Files
#include "TaskModel.h"

// QT Header File
#include "GuiTaskModel.h"
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
#include <memory>
#include <string>

GuiTaskModel::GuiTaskModel(QObject *parent)
    : QObject{parent},
    m_DbTaskId{0},
    m_CreatorUserId{0},
    m_AssigneeUserId{0},
    m_status{GuiTaskModel::GUITaskStatus::Not_Started},
    m_ParentTaskId{0},
    m_Personal{false},
    m_DbTaskDataPtr{nullptr},
    m_FieldsChangedValue{false}
{}

GuiTaskModel::GuiTaskModel(std::shared_ptr<TaskModel> dbTaskDataPtr, QObject* parent)
    : GuiTaskModel(parent)
{
    m_DbTaskDataPtr = dbTaskDataPtr;

    transferDbModelDataToFields();
}

GuiTaskModel::GuiTaskModel(std::size_t dbTaskId, QObject *parent)
    : GuiTaskModel(parent)
{
    TaskModel_shp newTaskModel = std::make_shared<TaskModel>();
    newTaskModel->setTaskID(dbTaskId);
    newTaskModel->retrieve();

    m_DbTaskDataPtr = newTaskModel;
    transferDbModelDataToFields();
}

void GuiTaskModel::setDbTaskId(std::size_t v)
{
    if (m_DbTaskId != v)
    {
        m_DbTaskId = v;
        m_FieldsChangedValue = true;
        emit dbTaskIdChanged();
    }
}

void GuiTaskModel::setCreatorUserId(std::size_t v)
{
    if (m_CreatorUserId != v)
    {
        m_CreatorUserId = v;
        m_FieldsChangedValue = true;
        emit creatorIdChanged();
    }
}

void GuiTaskModel::setAssigneeUserId(std::size_t v)
{
    if (m_AssigneeUserId != v)
    {
        m_AssigneeUserId = v;
        m_FieldsChangedValue = true;
        emit assigneeIdChanged();
    }
}

void GuiTaskModel::setPriorityGroup(QString v)
{
    if (m_PriorityGroup != v)
    {
        m_PriorityGroup = v;
        m_FieldsChangedValue = true;
        emit priorityGroupChanged();
    }
}

void GuiTaskModel::setPriority(QString v)
{
    if (m_Priority != v)
    {
        m_Priority = v;
        m_FieldsChangedValue = true;
        emit priorityChanged();
    }
}

void GuiTaskModel::setDescription(QString v)
{
    if (m_Description != v)
    {
        m_Description = v;
        m_FieldsChangedValue = true;
        emit descriptionChanged();
    }
}

void GuiTaskModel::setStatus(GUITaskStatus v)
{
    if (m_status != v)
    {
        m_status = v;
        m_FieldsChangedValue = true;
        emit statusChanged();
    }
}

void GuiTaskModel::setParentTaskId(std::size_t v)
{
    if (m_ParentTaskId != v)
    {
        m_ParentTaskId = v;
        m_FieldsChangedValue = true;
        emit parentTaskIdChanged();
    }
}

void GuiTaskModel::setDueDate(QDate v)
{
    if (m_DueDate != v)
    {
        m_DueDate = v;
        m_FieldsChangedValue = true;
        emit dueDateChanged();
    }
}

void GuiTaskModel::setScheduledStart(QDate v)
{
    if (m_ScheduledStart != v)
    {
        m_ScheduledStart = v;
        m_FieldsChangedValue = true;
        emit scheduledStartChanged();
    }
}

void GuiTaskModel::setActualStart(QDate v)
{
    if (m_ActualStartDate != v)
    {
        m_ActualStartDate = v;
        m_FieldsChangedValue = true;
        emit actualStartChanged();
    }
}

void GuiTaskModel::setEstimatedCompletion(QDate v)
{
    if (m_EstimatedCompletion != v)
    {
        m_EstimatedCompletion = v;
        m_FieldsChangedValue = true;
        emit estimatedCompletionChanged();
    }
}

void GuiTaskModel::setCompletionDate(QDate v)
{
    if (m_CompletionDate != v)
    {
        m_CompletionDate = v;
        m_FieldsChangedValue = true;
        emit completionDateChanged();
    }
}

void GuiTaskModel::setEstimatedEffort(QString v)
{
    if (m_EstimatedEffort != v)
    {
        m_EstimatedEffort = v;
        m_FieldsChangedValue = true;
        emit estimatedEffortChanged();
    }
}

void GuiTaskModel::setActualEffortToDate(QString v)
{
    if (m_ActualEffortToDate != v)
    {
        m_ActualEffortToDate = v;
        m_FieldsChangedValue = true;
        emit actualEffortToDateChanged();
    }
}

void GuiTaskModel::setPersonal(bool v)
{
    if (m_Personal != v)
    {
        m_Personal = v;
        m_FieldsChangedValue = true;
        emit personalChanged();
    }
}

bool GuiTaskModel::addTaskToDatabase()
{
    m_DbErrorMessages = "";

    m_DbTaskDataPtr = std::make_shared<TaskModel>();

    transferFieldsToDbModel();

    if (m_DbTaskDataPtr->insert())
    {
        m_DbTaskId = m_DbTaskDataPtr->getTaskID();
        return true;
    }

    m_DbErrorMessages = QString::fromStdString(m_DbTaskDataPtr->getAllErrorMessages());

    return false;
}

bool GuiTaskModel::updateTaskInDatabase()
{
    m_DbErrorMessages = "";

    return false;
}

GuiTaskModel::GUITaskStatus GuiTaskModel::convertFromTaskModel(std::size_t taskModelStatus)
{
    TaskModel::TaskStatus newStatus = static_cast<TaskModel::TaskStatus>(taskModelStatus);

    switch (newStatus)
    {
        case TaskModel::TaskStatus::Not_Started:
            return GuiTaskModel::GUITaskStatus::Not_Started;
        case TaskModel::TaskStatus::On_Hold :
            return GuiTaskModel::GUITaskStatus::On_Hold;
        case TaskModel::TaskStatus::Waiting_for_Dependency :
            return GuiTaskModel::GUITaskStatus::Waiting_for_Dependency;
        case TaskModel::TaskStatus::Work_in_Progress :
            return GuiTaskModel::GUITaskStatus::Work_in_Progress;
        case TaskModel::TaskStatus::Complete :
            return GuiTaskModel::GUITaskStatus::Complete;
        default:
            return GuiTaskModel::GUITaskStatus::UNKNOWN;
    }
}

std::size_t GuiTaskModel::TaskModelStatusFromGuiTaskStatus(GUITaskStatus taskStatus)
{
    switch (taskStatus)
    {
        default:
            return static_cast<std::size_t>(-1);
        case GuiTaskModel::GUITaskStatus::Complete :
            return static_cast<std::size_t>(TaskModel::TaskStatus::Complete);
        case GuiTaskModel::GUITaskStatus::Not_Started :
            return static_cast<std::size_t>(TaskModel::TaskStatus::Not_Started);
        case GuiTaskModel::GUITaskStatus::On_Hold :
            return static_cast<std::size_t>(TaskModel::TaskStatus::On_Hold);
        case GuiTaskModel::GUITaskStatus::Waiting_for_Dependency :
            return static_cast<std::size_t>(TaskModel::TaskStatus::Waiting_for_Dependency);
        case GuiTaskModel::GUITaskStatus::Work_in_Progress :
            return static_cast<std::size_t>(TaskModel::TaskStatus::Work_in_Progress);
    }
}
void GuiTaskModel::transferFieldsToDbModel()
{
    m_DbTaskDataPtr->setCreatorID(m_CreatorUserId);
    m_DbTaskDataPtr->setAssignToID(m_AssigneeUserId);
    m_DbTaskDataPtr->setDescription(m_Description.toStdString());
    m_status = convertFromTaskModel(static_cast<std::size_t>(m_DbTaskDataPtr->getStatus()));
    m_DbTaskDataPtr->setDueDate(qDateToChrono(m_DueDate));
    m_DbTaskDataPtr->setScheduledStart(qDateToChrono(m_ScheduledStart));

    transferOptionalFieldsToDBModel();

    bool numberisGood = false;
    unsigned int estimatedEffort = m_EstimatedEffort.toUInt(&numberisGood);
    if (numberisGood)
    {
        m_DbTaskDataPtr->setEstimatedEffort(estimatedEffort);
    }

    numberisGood = false;
    double actualEffort = m_ActualEffortToDate.toDouble(&numberisGood);
    if (numberisGood)
    {
        m_DbTaskDataPtr->setActualEffortToDate(actualEffort);
    }

    transferPriorityFieldDataToDBModel();
}

void GuiTaskModel::transferDbModelDataToFields()
{
    m_CreatorUserId = m_DbTaskDataPtr->getCreatorID();
    m_AssigneeUserId = m_DbTaskDataPtr->getAssignToID();
    m_Description = QString::fromStdString(m_DbTaskDataPtr->getDescription());
    m_status = convertFromTaskModel(static_cast<std::size_t>(m_DbTaskDataPtr->getStatus()));
    m_DueDate = QDate(m_DbTaskDataPtr->getDueDate());
    m_ScheduledStart = QDate(m_DbTaskDataPtr->getScheduledStart());

    if (m_DbTaskDataPtr->rawActualStartDate().has_value())
    {
        m_ActualStartDate = QDate(m_DbTaskDataPtr->getactualStartDate());
    }

    if (m_DbTaskDataPtr->rawEstimatedCompletion().has_value())
    {
        m_EstimatedCompletion = QDate(m_DbTaskDataPtr->getEstimatedCompletion());
    }

    if (m_DbTaskDataPtr->rawCompletionDate())
    {
        m_CompletionDate = QDate(m_DbTaskDataPtr->getCompletionDate());
    }

    m_EstimatedEffort = QString::fromStdString(std::to_string(m_DbTaskDataPtr->getEstimatedEffort()));

    m_ActualEffortToDate = QString::fromStdString(std::to_string(m_DbTaskDataPtr->getactualEffortToDate()));

    m_PriorityGroup = QString::fromStdString(std::to_string(m_DbTaskDataPtr->getPriorityGroup()));

    m_Priority = QString::fromStdString(std::to_string(m_DbTaskDataPtr->getPriority()));

    QDateTime tempqdt = chronoTimePointToQDateTime(m_DbTaskDataPtr->getCreationDate());
    m_CreationTimeStamp = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(m_DbTaskDataPtr->getLastUpdate());
    m_LastUpdateTimeStamp = tempqdt.toString(Qt::ISODate);
}

void GuiTaskModel::transferPriorityFieldDataToDBModel()
{
    bool numberisGood = false;
    unsigned int priorityGroup = m_PriorityGroup.toUInt(&numberisGood);
    if (numberisGood)
    {
        m_DbTaskDataPtr->setPriorityGroup(priorityGroup);
    }
    else
    {
        if (m_PriorityGroup.length() > 0)
        {
            QChar charPriorityGroup = m_PriorityGroup[0];
            m_DbTaskDataPtr->setPriorityGroupC(charPriorityGroup.toLatin1());
        }
    }

    numberisGood = false;
    unsigned int priority = m_Priority.toUInt(&numberisGood);
    if (numberisGood)
    {
        m_DbTaskDataPtr->setPriority(priority);
    }
}

void GuiTaskModel::transferOptionalFieldsToDBModel()
{
    if (m_ActualStartDate.isValid())
    {
        m_DbTaskDataPtr->setactualStartDate(qDateToChrono(m_ActualStartDate));
    }

    if (m_EstimatedCompletion.isValid())
    {
        m_DbTaskDataPtr->setEstimatedCompletion(qDateToChrono(m_EstimatedCompletion));
    }

    if (m_CompletionDate.isValid())
    {
        m_DbTaskDataPtr->setCompletionDate(qDateToChrono(m_CompletionDate));
    }
}

