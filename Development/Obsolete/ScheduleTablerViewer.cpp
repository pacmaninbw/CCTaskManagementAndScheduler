// Project Header Files
#include "GuiDashboardScheduleTable.h"
#include "UserModel.h"
#include "ScheduleTablerViewer.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QTableView>
#include <QWidget>

// Standard C++ Header Files

ScheduleTablerViewer::ScheduleTablerViewer(QWidget *parent)
    : QTableView{parent},
    m_DBUserId{0},
    m_DateOfSchedule{QDate::currentDate()},
    m_ScheduleTable{nullptr}
{
    m_ScheduleTable = new GuiDashboardScheduleTable(parent);
    m_ScheduleTable->setObjectName("m_ScheduleTable");

    setModel(m_ScheduleTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void ScheduleTablerViewer::setDate(QDate scheduleToShow)
{
    m_DateOfSchedule = scheduleToShow;
    if (m_ScheduleTable)
    {
        m_ScheduleTable->setDate(scheduleToShow);
    }
}

void ScheduleTablerViewer::setUserId(std::size_t userID)
{
    m_DBUserId = userID;
    if (m_ScheduleTable)
    {
        m_ScheduleTable->setUser(userID);
    }
}

void ScheduleTablerViewer::setUserIdAndDate(std::size_t userID, QDate scheduleToShow)
{
    m_DBUserId = userID;
    m_DateOfSchedule = scheduleToShow;
    if (m_ScheduleTable)
    {
        m_ScheduleTable->setUser(userID);
        m_ScheduleTable->setDate(scheduleToShow);
    }
}

void ScheduleTablerViewer::updateSchedule()
{
    if (!m_ScheduleTable)
    {
        m_ScheduleTable = new GuiDashboardScheduleTable(parent());
        m_ScheduleTable->setObjectName("m_ScheduleTable");
    }

    m_ScheduleTable->setUserAndDateRefillSchedule(m_DBUserId, m_DateOfSchedule);

    setModel(m_ScheduleTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

std::chrono::system_clock::time_point ScheduleTablerViewer::getScheduleItemStartTime(const QModelIndex &index)
{
    return m_ScheduleTable->getScheduleItemStartTime(index);
}

std::chrono::system_clock::time_point ScheduleTablerViewer::getScheduleItemEndTime(const QModelIndex &index)
{
    return m_ScheduleTable->getScheduleItemEndTime(index);
}
