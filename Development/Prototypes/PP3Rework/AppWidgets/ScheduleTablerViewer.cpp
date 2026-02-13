// Project Header Files
#include "GuiDashboardScheduleTable.h"
#include "GuiUserModel.h"
#include "ScheduleTablerViewer.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QTableView>
#include <QWidget>

// Standard C++ Header Files

ScheduleTablerViewer::ScheduleTablerViewer(QWidget *parent)
    : QTableView{parent},
    m_DBUserId{nullptr},
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

void ScheduleTablerViewer::setUserId(GuiUserModel *userDataPtr)
{
    m_DBUserId = userDataPtr;
    if (m_ScheduleTable)
    {
        m_ScheduleTable->setUser(userDataPtr);
    }
}

void ScheduleTablerViewer::setUserIdAndDate(GuiUserModel *userDataPtr, QDate scheduleToShow)
{
    m_DBUserId = userDataPtr;
    m_DateOfSchedule = scheduleToShow;
    if (m_ScheduleTable)
    {
        m_ScheduleTable->setUser(userDataPtr);
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
