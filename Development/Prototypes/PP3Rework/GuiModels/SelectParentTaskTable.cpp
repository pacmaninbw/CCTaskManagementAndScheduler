// Project Header Files
#include "GuiTaskModel.h"
#include "GuiUserModel.h"
#include "SelectParentTaskTable.h"

// QT Header Files

// Standard C++ Header Files


SelectParentTaskTable::SelectParentTaskTable(GuiUserModel *userDataPtr, QObject *parent)
    : GuiTaskTableBase(userDataPtr, parent)
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
    const GuiTaskModel* task = m_data[index.row()];

    switch (index.column()) {
        case 0: return QString::number(task->getDbTaskId());
        case 1: return task->getDescription();
        default: return {};
    }
}
