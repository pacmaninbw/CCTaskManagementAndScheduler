
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
    m_Personal{false}
{}

GuiTaskModel::GuiTaskModel(std::shared_ptr<TaskModel> dbTaskDataPtr, QObject* parent)
    : GuiTaskModel(parent)
{
    m_DbTaskDataPtr = dbTaskDataPtr;

    m_CreatorUserId = dbTaskDataPtr->getCreatorID();
    m_AssigneeUserId = dbTaskDataPtr->getAssignToID();
    m_Description = QString::fromStdString(dbTaskDataPtr->getDescription());
    m_status = convertFromTaskModel(static_cast<std::size_t>(dbTaskDataPtr->getStatus()));
    m_DueDate = QDate(dbTaskDataPtr->getDueDate());
    m_ScheduledStart = QDate(dbTaskDataPtr->getScheduledStart());

    if (dbTaskDataPtr->rawActualStartDate().has_value())
    {
        m_ActualStartDate = QDate(dbTaskDataPtr->getactualStartDate());
    }

    if (dbTaskDataPtr->rawEstimatedCompletion().has_value())
    {
        m_EstimatedCompletion = QDate(dbTaskDataPtr->getEstimatedCompletion());
    }

    if (dbTaskDataPtr->rawCompletionDate())
    {
        m_CompletionDate = QDate(dbTaskDataPtr->getCompletionDate());
    }

    m_EstimatedEffort = QString::fromStdString(std::to_string(dbTaskDataPtr->getEstimatedEffort()));

    m_ActualEffortToDate = QString::fromStdString(std::to_string(dbTaskDataPtr->getactualEffortToDate()));

    m_PriorityGroup = QString::fromStdString(std::to_string(dbTaskDataPtr->getPriorityGroup()));

    m_Priority = QString::fromStdString(std::to_string(dbTaskDataPtr->getPriority()));

    QDateTime tempqdt = chronoTimePointToQDateTime(dbTaskDataPtr->getCreationDate());
    m_CreationTimeStamp = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(dbTaskDataPtr->getLastUpdate());
    m_LastUpdateTimeStamp = tempqdt.toString(Qt::ISODate);
}

void GuiTaskModel::setDbTaskId(std::size_t v)
{
    if (m_DbTaskId != v)
    {
        m_DbTaskId = v;
        emit dbTaskIdChanged();
    }
}

void GuiTaskModel::setCreatorUserId(std::size_t v)
{
    if (m_CreatorUserId != v)
    {
        m_CreatorUserId = v;
        emit creatorIdChanged();
    }
}

void GuiTaskModel::setAssigneeUserId(std::size_t v)
{
    if (m_AssigneeUserId != v)
    {
        m_AssigneeUserId = v;
        emit assigneeIdChanged();
    }
}

void GuiTaskModel::setPriorityGroup(QString v)
{
    if (m_PriorityGroup != v)
    {
        m_PriorityGroup = v;
        emit priorityGroupChanged();
    }
}

void GuiTaskModel::setPriority(QString v)
{
    if (m_Priority != v)
    {
        m_Priority = v;
        emit priorityChanged();
    }
}

void GuiTaskModel::setDescription(QString v)
{
    if (m_Description != v)
    {
        m_Description = v;
        emit descriptionChanged();
    }
}

void GuiTaskModel::setStatus(GUITaskStatus v)
{
    if (m_status != v)
    {
        m_status = v;
        emit statusChanged();
    }
}

void GuiTaskModel::setParentTaskId(std::size_t v)
{
    if (m_ParentTaskId != v)
    {
        m_ParentTaskId = v;
        emit parentTaskIdChanged();
    }
}

void GuiTaskModel::setDueDate(QDate v)
{
    if (m_DueDate != v)
    {
        m_DueDate = v;
        emit dueDateChanged();
    }
}

void GuiTaskModel::setScheduledStart(QDate v)
{
    if (m_ScheduledStart != v)
    {
        m_ScheduledStart = v;
        emit scheduledStartChanged();
    }
}

void GuiTaskModel::setActualStart(QDate v)
{
    if (m_ActualStartDate != v)
    {
        m_ActualStartDate = v;
        emit actualStartChanged();
    }
}

void GuiTaskModel::setEstimatedCompletion(QDate v)
{
    if (m_EstimatedCompletion != v)
    {
        m_EstimatedCompletion = v;
        emit estimatedCompletionChanged();
    }
}

void GuiTaskModel::setCompletionDate(QDate v)
{
    if (m_CompletionDate != v)
    {
        m_CompletionDate = v;
        emit completionDateChanged();
    }
}

void GuiTaskModel::setEstimatedEffort(QString v)
{
    if (m_EstimatedEffort != v)
    {
        m_EstimatedEffort = v;
        emit estimatedEffortChanged();
    }
}

void GuiTaskModel::setActualEffortToDate(QString v)
{
    if (m_ActualEffortToDate != v)
    {
        m_ActualEffortToDate = v;
        emit actualEffortToDateChanged();
    }
}

void GuiTaskModel::setPersonal(bool v)
{
    if (m_Personal != v)
    {
        m_Personal = v;
        emit personalChanged();
    }
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
