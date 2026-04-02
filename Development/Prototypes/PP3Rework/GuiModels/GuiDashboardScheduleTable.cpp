// Project Header Files
#include "commonUtilities.h"
#include "ScheduleItemList.h"
#include "ScheduleItemModel.h"

// QT Header Files
#include "GuiDashboardScheduleTable.h"
#include "UserModel.h"
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTime>
#include <QTimeZone>
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>


GuiDashboardScheduleTable::GuiDashboardScheduleTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserID{0},
    m_DateOfSchedule{QDate::currentDate()}
{
    m_ChronDateOfSchedule = qDateToChrono(m_DateOfSchedule);
    fillSchedule();
}

GuiDashboardScheduleTable::GuiDashboardScheduleTable(std::size_t userID, QDate dateOfSchedule, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserID{userID},
    m_DateOfSchedule{dateOfSchedule}
{
    m_ChronDateOfSchedule = qDateToChrono(dateOfSchedule);
}

void GuiDashboardScheduleTable::setUser(std::size_t userID)
{
    m_UserID = userID;
}

void GuiDashboardScheduleTable::setDate(QDate dateOfSchedule)
{
    m_DateOfSchedule = dateOfSchedule;
}

void GuiDashboardScheduleTable::setUserAndDateRefillSchedule(std::size_t userID, QDate dateOfSchedule)
{
    m_UserID = userID;
    m_DateOfSchedule = dateOfSchedule;
    m_ChronDateOfSchedule = qDateToChrono(dateOfSchedule);

    fillSchedule();
}

void GuiDashboardScheduleTable::append(std::shared_ptr<ScheduleItemModel> scheduledItem)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

    m_data.push_back(scheduledItem);
    
    endInsertRows();
}

void GuiDashboardScheduleTable::clearData()
{
    m_ScheduledItems.clear();

    beginResetModel();

    m_data.clear();

    endResetModel();
}

std::chrono::system_clock::time_point GuiDashboardScheduleTable::getScheduleItemStartTime(const QModelIndex &index)
{
    std::chrono::system_clock::time_point startTime;

    if (!index.isValid())
    {
        return startTime;
    }

    std::shared_ptr<ScheduleItemModel> todoItem = m_data[index.row()];

    startTime = todoItem->getStartTime();

    return startTime;
}

std::chrono::system_clock::time_point GuiDashboardScheduleTable::getScheduleItemEndTime(const QModelIndex &index)
{
    std::chrono::system_clock::time_point endTime;

    if (!index.isValid())
    {
        return endTime;
    }

    std::shared_ptr<ScheduleItemModel> todoItem = m_data[index.row()];

    endTime = todoItem->getEndTime();

    return endTime;
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

    return m_data.size();
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
    const ScheduleItemModel* todoItem = m_data[index.row()].get();
    switch (index.column()) {
        case 0: {
            QDateTime startTime(chronoTimePointToQDateTime(todoItem->getStartTime()));
            return startTime.toLocalTime().time().toString("h:mm ap");
        }
        case 1: return QString::fromStdString(todoItem->getTitle());
        default: return {};
    }
}

bool GuiDashboardScheduleTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        Q_EMIT dataChanged(index, index, {role});
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

QModelIndex GuiDashboardScheduleTable::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    std::shared_ptr<ScheduleItemModel> scheduleItem = m_data.at(row);
    if (scheduleItem)
    {
        return createIndex(row, column, scheduleItem->getScheduleItemID());
    }

    return QModelIndex();
}

void GuiDashboardScheduleTable::fillSchedule()
{
    clearData();
    std::chrono::year_month_day dateOfSchedule(qDateToChrono(m_DateOfSchedule));

    if (m_UserID)
    {
        ScheduleItemList dbScheduleList(m_UserID);
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
        if (m_UserID)
        {
            scheduledItem->setUserID(m_UserID);
        }
        append(scheduledItem);
    }
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
