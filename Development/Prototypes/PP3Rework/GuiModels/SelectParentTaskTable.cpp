// Project Header Files
#include "SelectParentTaskTable.h"
#include "TaskModel.h"

// QT Header Files

// Standard C++ Header Files


SelectParentTaskTable::SelectParentTaskTable(std::size_t userID, QObject *parent)
    : GuiTaskTableBase(userID, parent)
{}

QVariant SelectParentTaskTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return {};
    }

    switch (section) {
        case 0: return "Task ID";
        case 1: return "Description";
        default: return {};
    }
}

int SelectParentTaskTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 2;
}

QVariant SelectParentTaskTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return {};
    }
    const TaskModel* task = m_data[index.row()].get();

    switch (index.column()) {
        case 0: return QString::number(task->getTaskID());
        case 1: return QString::fromStdString(task->getDescription());
        default: return {};
    }
}
