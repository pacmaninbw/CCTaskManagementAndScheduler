// Project Header Files
#include "GuiDashboardScheduleTable.h"
#include "GuiScheduleItemModel.h"
#include "GuiUserModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>
#include <QTime>

// Standard C++ Header Files
#include <ranges>


GuiDashboardScheduleTable::GuiDashboardScheduleTable(QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{nullptr},
    m_DateOfSchedule{QDate::currentDate()}
{
    makeBlankSchedule();
}

void GuiDashboardScheduleTable::setUser(GuiUserModel *userData)
{
    m_UserDataPtr = userData;
}

void GuiDashboardScheduleTable::setDate(QDate dateOfSchedule)
{
    m_DateOfSchedule = dateOfSchedule;
}

void GuiDashboardScheduleTable::setUserAndDate(GuiUserModel *userData, QDate dateOfSchedule)
{
    m_UserDataPtr = userData;
    m_DateOfSchedule = dateOfSchedule;
}

void GuiDashboardScheduleTable::fillSchedule()
{
    makeBlankSchedule();
    if (m_UserDataPtr)
    {
        addScheduledItems();
    }
}

QVariant GuiDashboardScheduleTable::headerData(int section,
                                               Qt::Orientation orientation,
                                               int role) const
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

void GuiDashboardScheduleTable::makeBlankSchedule()
{
    int dayStart = 8;

    if (!m_data.empty())
    {
        qDeleteAll(m_data.begin(), m_data.end());
        m_data.clear();
    }

    for (std::size_t hourInDay: std::views::iota(0, 12))
    {
        QTime scheduleTime(dayStart + hourInDay, 0, 0);
        QDateTime scheduleDateTime(m_DateOfSchedule, scheduleTime);
        std::size_t userId = (m_UserDataPtr)? m_UserDataPtr->getDbUserId() : 0;
        GuiScheduleItemModel* hourData = new GuiScheduleItemModel(userId, this->parent());
        hourData->setStartTime(scheduleDateTime);
        hourData->setTitle(" ");
        m_data.append(hourData);
    }
}

void GuiDashboardScheduleTable::addScheduledItems()
{

}
