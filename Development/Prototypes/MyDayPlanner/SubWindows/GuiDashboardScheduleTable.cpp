// Project Header Files
#include "commonUtilities.h"
#include "ScheduleItemModel.h"
#include "ScheduleItemQueryProcessor.h"
#include "stdChronoToQTConversions.h"

// QT Header Files
#include "GuiDashboardScheduleTable.h"
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTime>

// Standard C++ Header Files
#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>

GuiDashboardScheduleTable::GuiDashboardScheduleTable(std::size_t userID, QDate dateOfSchedule, QObject *parent)
    : QAbstractTableModel(parent),
    m_userID{userID},
    m_dateOfSchedule{dateOfSchedule}
{
    m_chronoDateOfSchedule = qDateToChrono(dateOfSchedule);
}

void GuiDashboardScheduleTable::append(std::shared_ptr<ScheduleItemModel> scheduledItem)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.push_back(scheduledItem);
    endInsertRows();
}

void GuiDashboardScheduleTable::clearData()
{
    m_scheduledItems.clear();

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

    std::shared_ptr<ScheduleItemModel> scheduleItem = m_data[index.row()];
    startTime = scheduleItem->getStartTime();

    return startTime;
}

std::chrono::system_clock::time_point GuiDashboardScheduleTable::getScheduleItemEndTime(const QModelIndex &index)
{
    std::chrono::system_clock::time_point endTime;

    if (!index.isValid())
    {
        return endTime;
    }

    std::shared_ptr<ScheduleItemModel> scheduleItem = m_data[index.row()];
    endTime = scheduleItem->getEndTime();

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
    const ScheduleItemModel* scheduleItem = m_data[index.row()].get();
    switch (index.column()) {
        case 0: {
            QDateTime startTime(chronoTimePointToQDateTime(scheduleItem->getStartTime()));
            return startTime.toLocalTime().time().toString("h:mm ap");
        }
        case 1: return QString::fromStdString(scheduleItem->getTitle());
        default: return {};
    }
}

bool GuiDashboardScheduleTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
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
    std::chrono::year_month_day dateOfSchedule(qDateToChrono(m_dateOfSchedule));

    if (m_userID)
    {
        ScheduleItemQueryProcessor dbScheduleList(m_userID);
        m_scheduledItems = dbScheduleList.getUserDaySchedule(dateOfSchedule);
    }

    setUserDay(dateOfSchedule);
    addBlankHoursForDisplay();
/*
 * Schedule items in the database may be in any order and we need to display
 * the schedule chronologically.
 */
    std::sort(m_scheduledItems.begin(), m_scheduledItems.end(),
        [](ScheduleItemModel_shp left, ScheduleItemModel_shp right)
        { return left->getStartTime() < right->getStartTime();}
    );

    for (const auto &scheduledItem: m_scheduledItems)
    {
        if (m_userID)
        {
            scheduledItem->setUserID(m_userID);
        }
        append(scheduledItem);
    }
}

/*
 * To correct from GMT time to local time we get the local midnight value and
 * offset from that. Currently the default start time is 08:00 or 8:00AM and
 * the default end time is 20:00 or 8:00PM. In future versions the user will
 * be able to set this in their preferences.
 */
void GuiDashboardScheduleTable::setUserDay(std::chrono::year_month_day scheduleDate)
{
    std::chrono::hours hour(1);
    m_userStartDay = getLocalMidnight(scheduleDate);
    // TODO get the users preferred start time and use that in the following calculation
    m_userStartDay += 8 * hour;
    // TODO get the users preferred end time and use that in the following calculation
    m_userEndDay = m_userStartDay + 12 * hour;
}

bool GuiDashboardScheduleTable::hasNoTimeConflicts(std::chrono::system_clock::time_point proposedHour)
{
    for (auto &scheduledItem: m_scheduledItems)
    {
        if (proposedHour >= scheduledItem->getStartTime() && proposedHour <= scheduledItem->getEndTime())
        {
            return false;
        }
    }

    return true;
}

/*
 * Hours that do not have events are not in the database. To provide a full view
 * of the users day schedule blank hours need to be added.
 */
void GuiDashboardScheduleTable::addBlankHoursForDisplay()
{
    const int secondsInHour = 3600;
    /*
     * To prevent any overlaps in the schedule the end of an hour is one less
     * second than a real hour.
     */
    const std::chrono::seconds endOfHour(secondsInHour - 1);
    const std::chrono::hours oneHour{1};
        
    std::chrono::system_clock::time_point proposedStartTime = std::chrono::floor<std::chrono::hours>(m_userStartDay);
    std::chrono::system_clock::time_point endDay = std::chrono::ceil<std::chrono::hours>(m_userEndDay);
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
            m_scheduledItems.push_back(blankHour);
        }
        proposedStartTime += oneHour;
    }
}
