
// Project Header Files
#include "TaskModel.h"

// QT Header File
#include "GuiTaskModel.h"

// Standard C++ Header Files
#include <memory>

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
}
