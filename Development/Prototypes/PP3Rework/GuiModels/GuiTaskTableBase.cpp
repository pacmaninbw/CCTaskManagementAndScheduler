// Project Header Files
#include "TaskList.h"
#include "GuiTaskTableBase.h"
#include "GuiTaskModel.h"
#include "GuiUserModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QList>
#include <QMetaEnum>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <ranges>


GuiTaskTableBase::GuiTaskTableBase(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{nullptr}
{

}

GuiTaskTableBase::GuiTaskTableBase(GuiUserModel *userDataPtr, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{userDataPtr}
{

}

void GuiTaskTableBase::setUserRefillTable(GuiUserModel *userDataPtr)
{
    m_UserDataPtr = userDataPtr;
    fillTable();
}

void GuiTaskTableBase::fillTable()
{
    if (!m_UserDataPtr || m_UserDataPtr->getDbUserId() == 0)
    {
        makeFakeQList();
        return;
    }

    std::chrono::year_month_day searchDate = getTodaysDatePlus(TwoWeeks);
    TaskList currentUserTaskList;
    TaskListValues userTasks = currentUserTaskList.getDefaultDashboardTaskList(m_UserDataPtr->getDbUserId(), searchDate);

    if (userTasks.empty())
    {
        makeFakeQList();
        return;
    }

    if (!m_data.empty())
    {
        qDeleteAll(m_data.begin(), m_data.end());
        m_data.clear();
    }
    for (const auto& dbTaskPtr: userTasks)
    {
        GuiTaskModel* modelData = new GuiTaskModel(dbTaskPtr, this);
        m_data.append(modelData);
    }
}

void GuiTaskTableBase::append(GuiTaskModel *taskData)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());

    m_data.append(taskData);

    endInsertRows();
}

void GuiTaskTableBase::clearData()
{
    beginResetModel();

    qDeleteAll(m_data.begin(), m_data.end());
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
        emit headerDataChanged(orientation, section, section);
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

    return m_data.count();
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
    const GuiTaskModel* task = m_data[index.row()];
    switch (index.column()) {
        case 0: return task->getPriorityGroup();
        case 1: return task->getPriorityGroup();
        case 2: return task->getPriority();
        case 3: return task->getDescription();
        case 4: return QString::number(task->getCreatorUserId());
        case 5: return QString::number(task->getAssigneeUserId());
        case 6: return QString::number(static_cast<int>(task->getStatus()));
        case 7: return QString::number(task->getParentTaskId());
        case 8: return task->getDueDate().toString(Qt::ISODate);
        case 9: return task->getScheduledStart().toString(Qt::ISODate);
        default: return {};
    }
}

bool GuiTaskTableBase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
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

bool GuiTaskTableBase::insertRows(int position, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position + count - 1);

    for (int row = 0; row < count; ++row)
    {
        m_data.insert(position, nullptr);
    }

    endInsertRows();

    return true;
}

bool GuiTaskTableBase::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool GuiTaskTableBase::removeRows(int position, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position + count - 1);

    for (int row = 0; row < count; ++row)
    {
        GuiTaskModel* data = m_data.at(position);
        delete data;
        m_data.removeAt(position);
    }

    endRemoveRows();

    return true;
}

bool GuiTaskTableBase::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

QModelIndex GuiTaskTableBase::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GuiTaskModel* taskModelItem = m_data.at(row);
    if (taskModelItem)
    {
        return createIndex(row, column, taskModelItem);
    }

    return QModelIndex();
}

void GuiTaskTableBase::makeFakeQList()
{
    if (!m_data.isEmpty())
    {
        return;
    }
    for (std::size_t priorityGroup: std::views::iota(0, 3))
    {
        QString priorityGroupString = "A";
        for (std::size_t priority : std::views::iota(1, 6))
        {
            GuiTaskModel* taskData = new GuiTaskModel(this->parent());
            taskData->setPriorityGroup(priorityGroupString);
            taskData->setPriority(QString::number(priority));
            taskData->setDescription(priorityGroup == 0 && priority == 1 ? "Login to app" : "");
            append(taskData);
        }
    }
}

