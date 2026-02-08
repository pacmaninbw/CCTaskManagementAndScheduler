// Project Header Files
#include "ScheduleItemList.h"

// QT Header Files
#include "GuiDashboardScheduleTable.h"
#include "GuiScheduleItemModel.h"
#include "GuiUserModel.h"
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>
#include <QTime>
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <ranges>


GuiDashboardScheduleTable::GuiDashboardScheduleTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{nullptr},
    m_DateOfSchedule{QDate::currentDate()}
{
    makeBlankSchedule();
}

GuiDashboardScheduleTable::GuiDashboardScheduleTable(GuiUserModel *userData, QDate dateOfSchedule, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{userData},
    m_DateOfSchedule{dateOfSchedule}
{
}

void GuiDashboardScheduleTable::setUser(GuiUserModel *userData)
{
    m_UserDataPtr = userData;
}

void GuiDashboardScheduleTable::setDate(QDate dateOfSchedule)
{
    m_DateOfSchedule = dateOfSchedule;
}

void GuiDashboardScheduleTable::setUserAndDateRefillSchedule(GuiUserModel *userData, QDate dateOfSchedule)
{
    m_UserDataPtr = userData;
    m_DateOfSchedule = dateOfSchedule;
    fillSchedule();
}

void GuiDashboardScheduleTable::append(GuiScheduleItemModel *scheduledItem)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());

    m_data.append(scheduledItem);
    
    endInsertRows();
}

void GuiDashboardScheduleTable::clearData()
{
    beginResetModel();

    qDeleteAll(m_data.begin(), m_data.end());
    m_data.clear();

    endResetModel();
}

QVariant GuiDashboardScheduleTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
        case 0: return "Start Time";
        case 1: return "Description";
        default: return {};
    }
}

int GuiDashboardScheduleTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.count();
}

int GuiDashboardScheduleTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 2;
}

QVariant GuiDashboardScheduleTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }


    if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
    const GuiScheduleItemModel* todoItem = m_data[index.row()];
    switch (index.column()) {
        case 0: return todoItem->getStartTime().time().toString("h:mm ap");
        case 1: return todoItem->getTitle();
        default: return {};
    }
}

bool GuiDashboardScheduleTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GuiDashboardScheduleTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool GuiDashboardScheduleTable::insertRows(int position, int count, const QModelIndex &parent)
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

bool GuiDashboardScheduleTable::removeRows(int position, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position + count - 1);

    for (int row = 0; row < count; ++row)
    {
        GuiScheduleItemModel* data = m_data.at(position);
        delete data;
        m_data.removeAt(position);
    }

    endRemoveRows();

    return true;
}

void GuiDashboardScheduleTable::fillSchedule()
{
    if (!m_data.empty())
    {
        clearData();
    }

    makeBlankSchedule();
    
    if (m_UserDataPtr)
    {
        addScheduledItems();
    }
}

void GuiDashboardScheduleTable::makeBlankSchedule()
{
    int dayStart = 8;

    for (std::size_t hourInDay: std::views::iota(0, 12))
    {
        QTime scheduleTime(dayStart + hourInDay, 0, 0);
        QDateTime scheduleDateTime(m_DateOfSchedule, scheduleTime);
        std::size_t userId = (m_UserDataPtr)? m_UserDataPtr->getDbUserId() : 0;
        GuiScheduleItemModel* hourData = new GuiScheduleItemModel(userId, this->parent());
        hourData->setStartTime(scheduleDateTime);
        hourData->setEndTime(scheduleDateTime.addSecs(3599));
        hourData->setTitle(" ");
        append(hourData);
    }
}

void GuiDashboardScheduleTable::addScheduledItems()
{
    if (!m_UserDataPtr)
    {
        return;
    }
    ScheduleItemList dbScheduleList(m_UserDataPtr->getDbUserId());

    std::chrono::year_month_day dateOfSchedule(qDateToChrono(m_DateOfSchedule));

    ScheduleItemListValues scheduledItems = dbScheduleList.getUserDaySchedule(dateOfSchedule);
    if (scheduledItems.size())
    {
        for (const auto &scheduledItem: scheduledItems)
        {
            GuiScheduleItemModel* newEntry = new GuiScheduleItemModel(scheduledItem, this->parent());
            append(newEntry);
        }
    }

    return;
}
