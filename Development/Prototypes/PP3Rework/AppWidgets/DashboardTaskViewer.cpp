// Project Header Files
#include "DashboardTaskViewer.h"
#include "GuiDashboardTaskTable.h"
#include "GuiUserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QWidget>
#include <QTableView>

// Standard C++ Header Files

DashboardTaskViewer::DashboardTaskViewer(QWidget *parent)
    : QTableView{parent},
    m_UserDataPtr{nullptr},
    m_DashboardDate{QDate::currentDate()},
    m_TaskTable{nullptr}
{
    m_TaskTable = new GuiDashboardTaskTable(parent);
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

void DashboardTaskViewer::setUserId(GuiUserModel *userDataPtr)
{
    m_UserDataPtr = userDataPtr;
    if (m_TaskTable)
    {
        m_TaskTable->setUserRefillTable(m_UserDataPtr);
    }
}

void DashboardTaskViewer::setUserIdAndDate(GuiUserModel *userDataPtr, QDate dashboardDate)
{
    m_UserDataPtr = userDataPtr;
    m_DashboardDate = dashboardDate;

    if (m_TaskTable)
    {
        m_TaskTable->setUserRefillTable(m_UserDataPtr);
    }
}

void DashboardTaskViewer::update()
{
    if (!m_TaskTable)
    {
        m_TaskTable =  (!m_UserDataPtr)? new GuiDashboardTaskTable(parent()) :
                new GuiDashboardTaskTable(m_UserDataPtr, parent());
        m_TaskTable->setObjectName("m_TaskTable");
        m_TaskTable->fillTable();
    }
    else
    {
        m_TaskTable->setUserRefillTable(m_UserDataPtr);
    }

    setModel(m_TaskTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

