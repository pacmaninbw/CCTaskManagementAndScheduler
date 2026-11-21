#include "BaseUserTable.h"

// Project Header Files
#include "commonUtilities.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <vector>


BaseUserTable::BaseUserTable(QObject *parent)
    : QAbstractTableModel{parent},
    m_searchOrganization{nullptr}
{

}

BaseUserTable::BaseUserTable(QDate searchDate, std::shared_ptr<OrganizationModel> searchOrganization, QObject *parent)
    : QAbstractTableModel{parent},
    m_searchDate{searchDate.toStdSysDays()},
    m_searchOrganization{searchOrganization}
{
}

void BaseUserTable::filltable()
{
    std::chrono::year_month_day searchDate = common::getTodaysDate();
    UserQueryProcessor userSource;
    UserModelList newUsers = userSource.getAllUsersAddedOn(searchDate);

    for (const auto& newUserptr: newUsers)
    {
        m_data.push_back(newUserptr);
    }
}

void BaseUserTable::append(UserModel_shp userData)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.push_back(userData);
    endInsertRows();
}

void BaseUserTable::clearData()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

UserModel_shp BaseUserTable::getDatabaseObject(const QModelIndex &index)
{
    UserModel_shp userModel = nullptr;

    if (index.isValid())
    {
        userModel = m_data[index.row()];
    }

    return userModel;
}

QVariant BaseUserTable::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 7: return "Date Added";
        case 8: return "Last Modified";
        case 9: return "Last Login";
        default: return {};
    }
}

int BaseUserTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.size();
}

int BaseUserTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 10;
}

QVariant BaseUserTable::data(const QModelIndex &index, int role) const
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
        case 7:
        {
            QDateTime createdDate = QDateTime::fromStdTimePoint(std::chrono::time_point_cast<std::chrono::milliseconds>(user->getCreatedTSValue()));
            return createdDate.toString("yyyy-MM-dd hh:mm");
        }
        case 8:
        {
            QDateTime lastModified = QDateTime::currentDateTime();
            return lastModified.toString("yyyy-MM-dd hh:mm");
        }
        case 9:
        {
            std::optional<std::chrono::system_clock::time_point> lastLogin = user->getLastLogin();
            if (lastLogin.has_value())
            {
                QDateTime formatted = QDateTime::fromStdTimePoint(std::chrono::time_point_cast<std::chrono::milliseconds>(lastLogin.value()));
                return formatted.toString("yyyy-MM-dd hh:mm");
            }
            else
            {
                return " ";
            }
        }
\
        default: return {};
    }
}

Qt::ItemFlags BaseUserTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QModelIndex BaseUserTable::index(int row, int column, const QModelIndex &parent) const
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

