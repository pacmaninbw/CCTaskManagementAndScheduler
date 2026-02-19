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
#include <QTimeZone>
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
#include <algorithm>
#include <chrono>


GuiDashboardScheduleTable::GuiDashboardScheduleTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{nullptr},
    m_DateOfSchedule{QDate::currentDate()}
{
    m_ChronDateOfSchedule = qDateToChrono(m_DateOfSchedule);
    fillSchedule();
}

GuiDashboardScheduleTable::GuiDashboardScheduleTable(GuiUserModel *userData, QDate dateOfSchedule, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{userData},
    m_DateOfSchedule{dateOfSchedule}
{
    m_ChronDateOfSchedule = qDateToChrono(dateOfSchedule);
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
    m_ChronDateOfSchedule = qDateToChrono(dateOfSchedule);

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
    m_ScheduledItems.clear();

    beginResetModel();

    qDeleteAll(m_data.begin(), m_data.end());
    m_data.clear();

    endResetModel();
}

QVariant GuiDashboardScheduleTable::headerData(int section, Qt::Orientation orientation, int role) const
{
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
        case 0: return todoItem->getStartTime().toLocalTime().time().toString("h:mm ap");
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
    {
        return Qt::NoItemFlags;
    }

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

QModelIndex GuiDashboardScheduleTable::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GuiScheduleItemModel* scheduleItem = m_data.at(row);
    if (scheduleItem)
    {
        return createIndex(row, column, scheduleItem);
    }

    return QModelIndex();
}

void GuiDashboardScheduleTable::fillSchedule()
{
    clearData();
    std::chrono::year_month_day dateOfSchedule(qDateToChrono(m_DateOfSchedule));

    if (m_UserDataPtr)
    {
        ScheduleItemList dbScheduleList(m_UserDataPtr->getDbUserId());
        m_ScheduledItems = dbScheduleList.getUserDaySchedule(dateOfSchedule);
    }

    setUserDay(dateOfSchedule);

    addBlankHoursForDisplay();

    std::sort(m_ScheduledItems.begin(), m_ScheduledItems.end(),
        [](ScheduleItemModel_shp left, ScheduleItemModel_shp right)
        { return left->getStartTime() < right->getStartTime();}
    );

    for (const auto &scheduledItem: m_ScheduledItems)
    {
        GuiScheduleItemModel* newEntry = new GuiScheduleItemModel(scheduledItem, this->parent());
        if (m_UserDataPtr)
        {
            newEntry->setUserID(m_UserDataPtr->getDbUserId());
        }
        append(newEntry);
    }
}

std::chrono::system_clock::time_point GuiDashboardScheduleTable::getLocalMidnight(std::chrono::year_month_day scheduleDate)
{
    const std::chrono::time_zone* localTimeZone = std::chrono::current_zone();
    const std::chrono::local_days local_midnight{scheduleDate};
    const std::chrono::zoned_time zt_midnight = localTimeZone->to_sys(local_midnight);
    return zt_midnight.get_sys_time();
}

void GuiDashboardScheduleTable::setUserDay(std::chrono::year_month_day scheduleDate)
{
    std::chrono::hours hour(1);
    m_UserStartDay = getLocalMidnight(scheduleDate);
    // TODO get the users preferred start time and use that in the following calculation
    m_UserStartDay += 8 * hour;
    // TODO get the users preferred end time and use that in the following calculation
    m_UserEndDay = m_UserStartDay + 12 * hour;
}

bool GuiDashboardScheduleTable::hasNoTimeConflicts(std::chrono::system_clock::time_point proposedHour)
{
    for (auto &scheduledItem: m_ScheduledItems)
    {
        if (proposedHour >= scheduledItem->getStartTime() && proposedHour <= scheduledItem->getEndTime())
        {
            return false;
        }
    }

    return true;
}

void GuiDashboardScheduleTable::addBlankHoursForDisplay()
{
    const int secondsInHour = 3600;
    const std::chrono::seconds endOfHour(secondsInHour - 1);
    const std::chrono::hours oneHour{1};
        
    std::chrono::system_clock::time_point proposedStartTime = std::chrono::floor<std::chrono::hours>(m_UserStartDay);
    std::chrono::system_clock::time_point endDay = std::chrono::ceil<std::chrono::hours>(m_UserEndDay);
    while (proposedStartTime < endDay)
    {
        if (hasNoTimeConflicts(proposedStartTime))
        {
            ScheduleItemModel_shp blankHour = std::make_shared<ScheduleItemModel>();
            blankHour->setStartDateAndTime(proposedStartTime);
            blankHour->setEndDateAndTime(proposedStartTime + endOfHour);
            blankHour->setTitle(" ");
            blankHour->setCreationDate(std::chrono::system_clock::now());
            blankHour->setLastUpdate(std::chrono::system_clock::now());
            m_ScheduledItems.push_back(blankHour);
        }
        proposedStartTime += oneHour;
    }

}
