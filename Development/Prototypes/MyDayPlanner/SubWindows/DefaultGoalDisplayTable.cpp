#include "DefaultGoalDisplayTable.h"

// Project Header Files
#include "UserGoalModel.h"
#include "GoalQueryProcessor.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QString>

// Standard C++ Header Files
#include <memory>
#include <vector>

DefaultGoalDisplayTable::DefaultGoalDisplayTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_userID{0}
{}

DefaultGoalDisplayTable::DefaultGoalDisplayTable(std::size_t userID, QObject *parent)
    : QAbstractTableModel(parent),
    m_userID{userID}
{

}

void DefaultGoalDisplayTable::setUserRefillGoalTable(std::size_t userID)
{
    m_userID = userID;

    fillGoalTable();
}

void DefaultGoalDisplayTable::refillTable()
{
    fillGoalTable();
}

void DefaultGoalDisplayTable::append(std::shared_ptr<UserGoalModel> goalItem)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

    m_data.push_back(goalItem);

    endInsertRows();
}

void DefaultGoalDisplayTable::clearData()
{
    m_goalList.clear();

    beginResetModel();

    m_data.clear();

    endResetModel();
}

QVariant DefaultGoalDisplayTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
        case 0: return "Priority";
        case 1: return "Description";
        case 2: return "Parent";
        default: return {};
    }
}

bool DefaultGoalDisplayTable::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        Q_EMIT headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int DefaultGoalDisplayTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.size();
}

int DefaultGoalDisplayTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 3;
}

QVariant DefaultGoalDisplayTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) return {};

    const UserGoalModel* goal = m_data[index.row()].get();

    switch (index.column())
    {
        default:
            return {};
        case 0:
        {
        unsigned int priority = goal->getPriority();
            if (priority > 0)
            {
                return QString::number(priority);
            }
            else
            {
                return QString(" ");
            }
        }
        case 1:
            return QString::fromStdString(goal->getDescription());
        case 2:
            return QString::number(goal->getParentId());
    }

    return QVariant();
}

bool DefaultGoalDisplayTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        Q_EMIT dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags DefaultGoalDisplayTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool DefaultGoalDisplayTable::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool DefaultGoalDisplayTable::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

QModelIndex DefaultGoalDisplayTable::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    UserGoalModel_shp goalModelItem = m_data[row];
    if (goalModelItem)
    {
        return createIndex(row, column, goalModelItem->getGoalId());
    }

    return QModelIndex();
}

void DefaultGoalDisplayTable::fillGoalTable()
{
    clearData();

    if (m_userID)
    {
        GoalQueryProcessor goalQueryProcessor;
        m_goalList = goalQueryProcessor.getAllGoalsForUser(m_userID);
    }

    if (!m_goalList.empty())
    {
        for (const auto &goalItem: m_goalList)
        {
            append(goalItem);
        }
    }
    else
    {
        UserGoalModel_shp addGoals = std::make_shared<UserGoalModel>();
        addGoals->setDescription("There are no goals to display.");
        append(addGoals);
    }

}
