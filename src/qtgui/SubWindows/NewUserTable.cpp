
// Project Header Files
#include "commonUtilities.h"
#include "NewUserTable.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <vector>


NewUserTable::NewUserTable(QObject *parent)
    : QAbstractTableModel{parent}
{}

void NewUserTable::refillTable()
{

}

void NewUserTable::fillTable()
{
    std::chrono::year_month_day searchDate = common::getTodaysDate();
    UserQueryProcessor userSource;
    UserModelList newUsers = userSource.getAllUsersAddedOn(searchDate);

    for (const auto& newUserptr: newUsers)
    {
        m_data.push_back(newUserptr);
    }
}

void NewUserTable::append(std::shared_ptr<UserModel> userData)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

    m_data.push_back(userData);

    endInsertRows();
}

void NewUserTable::clearData()
{
    beginResetModel();

    m_data.clear();

    endResetModel();
}

std::shared_ptr<UserModel> NewUserTable::getDatabaseObject(const QModelIndex &index)
{
    UserModel_shp userModel = nullptr;

    if (index.isValid())
    {
        userModel = m_data[index.row()];
    }

    return userModel;
}

QVariant NewUserTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};

    switch (section)
    {
        case 0: return "User ID";
        case 1: return "User Login";
        case 2: return "User eMail";
        case 3: return "Org ID";
        case 4: return "Last Name";
        case 5: return "First Name";
        case 6: return "Middle Inital";
        default: return {};
    }
}

int NewUserTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.size();
}

int NewUserTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 7;
}

QVariant NewUserTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }


    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return {};
    }

    UserModel_shp user = m_data[index.row()];
    switch (index.column())
    {
        case 0: return QString::number(user->getUserID());
        case 1: return QString::fromStdString(user->getLoginName());
        case 2: return QString::fromStdString(user->getEmail());
        case 3: return QString::number(user->getOrganizationID());
        case 4: return QString::fromStdString(user->getLastName());
        case 5: return QString::fromStdString(user->getFirstName());
        case 6: return QString::fromStdString(user->getMiddleInitial());

        default: return {};
    }
}

Qt::ItemFlags NewUserTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QModelIndex NewUserTable::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    UserModel_shp userModelItem = m_data[row];
    if (userModelItem)
    {
        return createIndex(row, column, userModelItem->getUserID());
    }

    return QModelIndex();
}
