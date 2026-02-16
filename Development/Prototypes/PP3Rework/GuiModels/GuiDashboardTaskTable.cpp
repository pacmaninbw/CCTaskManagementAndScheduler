// Project Header Files
#include "TaskList.h"
#include "GuiDashboardTaskTable.h"
#include "GuiTaskModel.h"
#include "GuiUserModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <ranges>


GuiDashboardTaskTable::GuiDashboardTaskTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{nullptr}
{}

GuiDashboardTaskTable::GuiDashboardTaskTable(GuiUserModel *userDataPtr, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{userDataPtr}
{
}

void GuiDashboardTaskTable::setUserRefillTable(GuiUserModel *userDataPtr)
{
    m_UserDataPtr = userDataPtr;
    fillTable();
}

void GuiDashboardTaskTable::fillTable()
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

void GuiDashboardTaskTable::append(GuiTaskModel* taskData)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());

    m_data.append(taskData);
    
    endInsertRows();

}

void GuiDashboardTaskTable::clearData()
{
    beginResetModel();

    qDeleteAll(m_data.begin(), m_data.end());
    m_data.clear();

    endResetModel();
}

QVariant GuiDashboardTaskTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
        case 0: return "PG";
        case 1: return "Priority";
        case 2: return "Description";
        default: return {};
    }
}

bool GuiDashboardTaskTable::setHeaderData(
        int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int GuiDashboardTaskTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.count();
}

int GuiDashboardTaskTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 3;
}

QVariant GuiDashboardTaskTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }


    if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
    const GuiTaskModel* task = m_data[index.row()];
    switch (index.column()) {
        case 0: return task->getPriorityGroup();
        case 1: return task->getPriority();
        case 2: return task->getDescription();
        default: return {};
    }
}

bool GuiDashboardTaskTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GuiDashboardTaskTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool GuiDashboardTaskTable::insertRows(int position, int count, const QModelIndex &parent)
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


bool GuiDashboardTaskTable::removeRows(int position, int count, const QModelIndex &parent)
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

QModelIndex GuiDashboardTaskTable::index(int row, int column, const QModelIndex &parent) const
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

void GuiDashboardTaskTable::makeFakeQList()
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
