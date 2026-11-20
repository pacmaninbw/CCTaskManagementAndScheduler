// Project Header Files
#include "DashboardTaskViewer.h"
#include "GuiDashboardTaskTable.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QWidget>
#include <QTableView>

// Standard C++ Header Files

DashboardTaskViewer::DashboardTaskViewer(QWidget *parent)
    : QTableView{parent},
    m_UserID{0},
    m_DashboardDate{QDate::currentDate()},
    m_TaskTable{nullptr}
{
    m_TaskTable = new GuiDashboardTaskTable(m_UserID, parent);
    m_TaskTable->setObjectName("m_TaskTable");
    m_TaskTable->fillTable();

    setModel(m_TaskTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void DashboardTaskViewer::setDate(QDate dashboardDate)
{
    m_DashboardDate = dashboardDate;
}

void DashboardTaskViewer::setUserId(std::size_t userID)
{
    m_UserID = userID;
    if (m_TaskTable)
    {
        m_TaskTable->setUserRefillTable(m_UserID);
    }
}

void DashboardTaskViewer::setUserIdAndDate(std::size_t userID, QDate dashboardDate)
{
    m_UserID = userID;
    m_DashboardDate = dashboardDate;

    if (m_TaskTable)
    {
        m_TaskTable->setUserRefillTable(m_UserID);
    }
}

void DashboardTaskViewer::update()
{
    if (m_TaskTable)
    {
        delete m_TaskTable;
    }
 
    m_TaskTable = new GuiDashboardTaskTable(m_UserID, parent());
    m_TaskTable->setObjectName("m_TaskTable");
    m_TaskTable->fillTable();

    setModel(m_TaskTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

