#include "GuiDashboardTaskTable.h"

GuiDashboardTaskTable::GuiDashboardTaskTable(QObject *parent)
    : QAbstractTableModel(parent)
{}

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
        return 0;

    return m_data.count();
}

int GuiDashboardTaskTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 3;
}

QVariant GuiDashboardTaskTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
    const auto &task = m_data[index.row()];
    switch (index.column()) {
        case 0: return task.getPriorityGroup();
        case 1: return task.getPriority();
        case 2: return task.getDescription();
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

bool GuiDashboardTaskTable::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool GuiDashboardTaskTable::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool GuiDashboardTaskTable::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool GuiDashboardTaskTable::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
