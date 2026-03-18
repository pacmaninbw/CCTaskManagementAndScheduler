// Project Header Files
#include "GuiTaskTableBase.h"
#include "TaskList.h"
#include "TaskModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QList>
#include <QMetaEnum>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <ranges>
#include <vector>

GuiTaskTableBase::GuiTaskTableBase(std::size_t userID, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserID{userID}
{

}

void GuiTaskTableBase::setUserRefillTable(std::size_t userID)
{
    m_UserID = userID;
    fillTable();
}

void GuiTaskTableBase::fillTable()
{
    if (m_UserID == 0)
    {
        makeFakeQList();
        return;
    }

    std::chrono::year_month_day searchDate = getTodaysDatePlus(TwoWeeks);
    TaskList currentUserTaskList;
    TaskListValues userTasks = currentUserTaskList.getDefaultDashboardTaskList(m_UserID, searchDate);

    if (userTasks.empty())
    {
        makeFakeQList();
        return;
    }

    if (!m_data.empty())
    {
        m_data.clear();
    }

    m_data = userTasks;
}

void GuiTaskTableBase::clearData()
{
    beginResetModel();

    m_data.clear();

    endResetModel();
}

QVariant GuiTaskTableBase::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
        case 0: return "Task ID";
        case 1: return "Category";
        case 2: return "Priority";
        case 3: return "Description";
        case 4: return "Creator ID";
        case 5: return "Assigned ID";
        case 6: return "Status";
        case 7: return "Parent ID";
        case 8: return "Due Date";
        case 9: return "Scheduled Start";
        default: return {};
    }
}

bool GuiTaskTableBase::setHeaderData(int section,
        Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        Q_EMIT headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int GuiTaskTableBase::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.size();
}

int GuiTaskTableBase::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 10;
}

bool GuiTaskTableBase::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // FIXME: Implement me!
    return false;
}

bool GuiTaskTableBase::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // FIXME: Implement me!
    return false;
}

void GuiTaskTableBase::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    // FIXME: Implement me!
}

QVariant GuiTaskTableBase::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }


    if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
    const TaskModel* task = m_data[index.row()].get();
    switch (index.column()) {
        case 0: return task->getPriorityGroup();
        case 1: return task->getPriorityGroup();
        case 2: return task->getPriority();
        case 3: return QString::fromStdString(task->getDescription());
        case 4: return QString::number(task->getCreatorID());
        case 5: return QString::number(task->getAssignToID());
        case 6: return QString::number(static_cast<int>(task->getStatus()));
        case 7: return QString::number(task->getParentTaskID());
        case 8: return QDate(task->getDueDate()).toString(Qt::ISODate);
        case 9: return QDate(task->getScheduledStart()).toString(Qt::ISODate);
        default: return {};
    }
}

bool GuiTaskTableBase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        Q_EMIT dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GuiTaskTableBase::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QModelIndex GuiTaskTableBase::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TaskModel_shp taskModelItem = m_data.at(row);
    if (taskModelItem)
    {
        return createIndex(row, column, taskModelItem->getTaskID());
    }

    return QModelIndex();
}

void GuiTaskTableBase::makeFakeQList()
{
    if (!m_data.empty())
    {
        return;
    }

    for (std::size_t priorityGroup: std::views::iota(0, 3))
    {
        QString priorityGroupString = "A";
        for (std::size_t priority : std::views::iota(1, 6))
        {
            TaskModel_shp taskData = std::make_shared<TaskModel>();
            taskData->setPriorityGroup(priorityGroup);
            taskData->setPriority(priority);
            taskData->setDescription(priorityGroup == 0 && priority == 1 ? "Login to app" : "");
            m_data.push_back(taskData);
        }
    }
}

