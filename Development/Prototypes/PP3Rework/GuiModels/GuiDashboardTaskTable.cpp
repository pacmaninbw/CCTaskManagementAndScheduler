// Project Header Files
#include "GuiDashboardTaskTable.h"
#include "GuiTaskModel.h"
#include "GuiUserModel.h"

// QT Header Files

// Standard C++ Header Files


GuiDashboardTaskTable::GuiDashboardTaskTable(QObject *parent)
    : GuiTaskTableBase(parent)
{}

GuiDashboardTaskTable::GuiDashboardTaskTable(GuiUserModel *userDataPtr, QObject *parent)
    : GuiTaskTableBase(userDataPtr, parent)
{
}

QVariant GuiDashboardTaskTable::headerData(int section, Qt::Orientation orientation, int role) const
{
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

#if 0
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
#endif
